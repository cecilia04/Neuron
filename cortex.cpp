#include "cortex.hpp"
#include <cassert>
#include <random>
#include <algorithm>

Cortex::Cortex() {}
	
Cortex::~Cortex() {
	deleteNeurons();
} 
	
Cortex::Cortex(Cortex const& another)
	:neurons_(another.neurons_) {}

/** Initialization of the neurons in the cortex, their g, eta, J and size of their buffer
 * @param time to initialize the neuron clock (time at which it is created)
 * @param h time step size
 * @param g =Je/Ji
 * @param eta =nu_ext/nu_thr
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
	
	assert(neurons_.size() == nb_neurons_);
}

/** Initilization of all the connections between neurons
 * Each neuron receive input nb_connections_exc_ from  random excitatory neurons 
 * and nb_connections_inhib_ from random inhibitory neurons
 * Random neurons are chosen with a uniform distribution 
 */
void Cortex::initConnections() {
	
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<> dis_e(0, nb_excitatory_ - 1);
	static std::uniform_int_distribution<> dis_i(nb_excitatory_, nb_neurons_ - 1);
	
	for (unsigned int i(0); i < nb_neurons_;++i) {
		for (unsigned int j(0); j < nb_connections_exc_; ++j) {
			neurons_[dis_e(gen)]->fillTargets(i);
		}
		for (unsigned int k(0); k < nb_connections_inhib_; ++k) {
			neurons_[dis_i(gen)]->fillTargets(i);
		}
	}
}


/**updates all the neurons in the cortex during a given time interval
 * @param h time step size
 * @param step_start step when we start updating
 * @param step_stop step when we stop updating
 */
void Cortex::updateNeurons(double h, long step_start, long step_stop) {
	
	assert(step_start >= 0);
	assert(step_stop >= 0);
	assert(step_start <= step_stop);
	
	for (auto n : neurons_) {
		n->computeConstants(h); /*! computes c1_, c2_ and nu_ext_ for each neurons */
	}
	
	static std::random_device rd;
	static std::mt19937 gen(rd());
	
	size_t s = neurons_[0]->getBuffer().size(); /*! calculates the size of the buffer, all the neurons have the same size */
	size_t D = s-1; //the buffer size is delay + 1 
	
	while (step_start < step_stop) {    /*! For each time step between step_start and step_stop */
		if(step_start%100==0) {std::cout << step_start << std::endl;}
		
		auto W = (step_start + D) % s; /*! calculates where we Write in the buffer, same for all neurons */
		assert(W < s);
		
		for (size_t i(0); i < neurons_.size(); ++i) {
			
			static std::poisson_distribution<> dis(neurons_[i]->getNuExt() * h); 
			
			if (neurons_[i]->update(h, step_start, dis(gen))) { 		/*! We update every neuron and if the neuron i had a spike */
				for (auto target : neurons_[i]->getTargets()) { 	/*! Each of his targets receives J in their buffer */
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

/** Saves in a file when each neuron had a spike */
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
