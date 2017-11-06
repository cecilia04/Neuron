#include "cortex.hpp"
#include <cassert>
#include <random>
#include <algorithm>

Cortex::Cortex() {} /*! constructor */
	
/** destructor */
Cortex::~Cortex() {
	deleteNeurons();
} 
	
Cortex::Cortex(Cortex const& another) /*! copy constructor */
	:neurons_(another.neurons_) {}

/** Initialization of the neurons in the cortex
 * @param time to initialize the neuron clock (time at which it is created)
 * @param h time step size
 */
void Cortex::initNeurons(double time, double h, double g, double eta) {
	for (unsigned int i(0); i < nb_excitatory_; ++i) {
		neurons_.push_back(new Neuron);
		neurons_[i]->setClock(time);
		neurons_[i]->resizeBuffer(neurons_[i]->getDelay() / h + 1);
		neurons_[i]->setG(g);
		neurons_[i]->setEta(eta);
		neurons_[i]->setJ(0.1);
	}
	
	for (unsigned int i(nb_excitatory_); i < nb_neurons_; ++i) {
		neurons_.push_back(new Neuron);
		neurons_[i]->setClock(time);
		neurons_[i]->resizeBuffer(neurons_[i]->getDelay() / h + 1);
		neurons_[i]->setG(g);
		neurons_[i]->setEta(eta);
		neurons_[i]->setJ(-0.1 * g);
	}
}

/** Initilization of all the connections between neurons */
void Cortex::initConnections() {
	for (unsigned int i(0); i < nb_neurons_;++i) {
		for (unsigned int j(0); j < nb_connections_exc_; ++j) {
			neurons_[random_uniform(0, nb_excitatory_)]->fillTargets(i);
		}
		for (unsigned int k(0); k < nb_connections_inhib_; ++k) {
			neurons_[random_uniform(nb_excitatory_, nb_neurons_)]->fillTargets(i);
		}
	}
}


/**updates all the neurons in the cortex
 * @param output file where we write membrane potential
 * @param h time step size
 * @param step number of the step we are in
 */
void Cortex::updateNeurons(double h, long step_start, long step_stop) {
	for (auto n : neurons_) {
		n->computeConstants(h);
	}
	
	size_t s = neurons_[0]->getBuffer().size(); /*! calculates the size of the buffer, all the neurons have the same size */
	
	while (step_start < step_stop) {
		if(step_start%100==0)
			std::cout << step_start << std::endl;
		
		auto W = (step_start + s-1) % s; /*! where we Write in the buffer, same for all neurons */
		assert(W < s);
		
		for (size_t i(0); i < neurons_.size(); ++i) {
			bool spike(neurons_[i]->update(h, step_start)); /*! updates the neuron i */
			if (!neurons_[i]->isRefractory()) {
				neurons_[i]->setPotentialPoisson(h);
			}
			
			if (spike) { /*! if the neuron i had a spike */
				for (auto target : neurons_[i]->getTargets()) {
					neurons_[target]->setBuffer(W, neurons_[i]->getJ());
				}
			}
		}
	++step_start;
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
	
void Cortex::deleteNeurons() {
	for (auto& neuron : neurons_) {
		delete neuron;
		neuron = nullptr;
	}
	
	neurons_.clear();
}

int Cortex::random_uniform(unsigned int start, unsigned int stop) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<> dis(start, stop-1);
	
	return dis(gen);
}
