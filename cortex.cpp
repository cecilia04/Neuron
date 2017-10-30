#include "cortex.hpp"
#include <cassert>
#include <random>

Cortex::Cortex() {} /*! constructor */
	
/** destructor */
Cortex::~Cortex() {
	for (auto& neuron : neurons_) {
		delete neuron;
		neuron = nullptr;
	}
	
	neurons_.clear();
} 
	
Cortex::Cortex(Cortex const& another) /*! copy constructor */
	:neurons_(another.neurons_) {}
	
/** Initialization of the neurons in the cortex
 * @param time to initialize the neuron clock (time at which it is created)
 * @param h time step size
 */
void Cortex::initNeurons(double time, double h) {
	for (unsigned int i(0); i < nb_excitatory_; ++i) {
		neurons_.push_back(new Neuron);
		neurons_[i]->setClock(time);
		neurons_[i]->resizeBuffer(neurons_[i]->getDelay() / h + 1);
		neurons_[i]->setJ(0.1);
	}
	
	for (unsigned int i(nb_excitatory_); i < nb_neurons_; ++i) {
		neurons_.push_back(new Neuron);
		neurons_[i]->setClock(time);
		neurons_[i]->resizeBuffer(neurons_[i]->getDelay() / h + 1);
		neurons_[i]->setJ(0.5);
	}
}

/** Initilization of all the connections between neurons */
void Cortex::initConnections() {
	for (unsigned int i(0); i < nb_neurons_;++i) {
		connections_.push_back({});   /*! creates an empty vector */
		for (unsigned int j(0); j < nb_neurons_; ++j) {
			connections_[i].push_back(0);
		}
	}
	
	std::cout << "connections matrix created" << std::endl;
	
	for (unsigned int i(0); i < nb_excitatory_; ++i) {
		for (unsigned int k(0); k < nb_connections_exc_; ++k) { 
			int rand = random_uniform(nb_neurons_);
			++(connections_[i][rand]);
		}
	}
	
	std::cout << "connections made for excitatory neurons" << std::endl;
	
	for (unsigned int i(nb_excitatory_); i < nb_neurons_; ++i) {
		for (unsigned int k(0); k < nb_connections_inhib_; ++k) { 
			int rand = random_uniform(nb_neurons_);
			++connections_[i][rand];
		}
	}
	
	std::cout << "connections made for inhibitory neurons" << std::endl;
}

/**updates all the neurons in the cortex
 * @param output file where we write membrane potential
 * @param h time step size
 * @param step number of the step we are in
 */
void Cortex::updateNeurons(std::ofstream & output, double h, long step) {
	for (size_t i(0); i < neurons_.size(); ++i) {
			
		output << "Data for neuron : " << i+1;
		bool spike(neurons_[i]->update(h, step)); /*! updates the neuron i */
		
		if (neurons_[i]->getRefractorySteps() <= 0.0) {
			neurons_[i]->setPotentialPoisson();
		}
		
		output << "Time : " << neurons_[i]->getClock() << " ms; Membrane potential : " << neurons_[i]->getPotential() << " mV" << "\n";	
			
		if (spike and (!connections_.empty())) { /*! if the neuron i had a spike */
			for (size_t j(0); j < nb_neurons_; ++j) {
				size_t s = neurons_[j]->getBuffer().size(); /*! calculates the size of the buffer */
				const auto W = (step + s-1) % s; /*! where we Write in the buffer */
				assert(W < s);
				neurons_[j]->setBuffer(W, connections_[i][j]); /*! the neuron j stores n*J in his buffer if it is connected n times to neuron i */
			}
		}
	}
}	

/** Displays when spikes occured for each neuron */	
void Cortex::printTimeSpikes() {
	for (size_t i(0); i < neurons_.size(); ++i) {
		std::cout << " Times when the spikes occured for neuron " << i+1 << " : " << std::endl;
		for (auto time : neurons_[i]->getTimeSpikes()) {
			std::cout << time << std::endl;
		}
	}
}

void Cortex::saveToFile(std::ofstream & file) {
	for (unsigned int i(0); i < nb_neurons_ ; ++i) {
		for (size_t j(0); j < neurons_[i]->getTimeSpikes().size(); ++j) {
			file << neurons_[i]->getTimeSpikes()[j] << "\t" << i << "\n";
		}
	}
}

void Cortex::setNeuronInput(size_t i, double input) {
	neurons_[i]->setInput(input);
}

/** Generates a random number with a uniform distribution
 * @param n the generated number must be below n-1
 */
int Cortex::random_uniform(unsigned int n) {
	std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, n-1);
    
    return dis(gen);
}	
	
	
