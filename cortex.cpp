#include "cortex.hpp"
#include <cassert>
#include <random>

Cortex::Cortex() {}; //constructor
	
//destructor
Cortex::~Cortex() {
	for (auto& neuron : neurons_) {
		delete neuron;
		neuron = nullptr;
	}
	
	neurons_.clear();
}; 
	
Cortex::Cortex(Cortex const& another) //copy constructor
	:neurons_(another.neurons_) {};
	
//initialization of the neurons in the cortex
void Cortex::initNeurons(double time, double h) {
	for (unsigned int i(0); i < nb_neurons_; ++i) {
		neurons_.push_back(new Neuron);
		neurons_[i]->setClock(time);
		neurons_[i]->resizeBuffer(neurons_[i]->getDelay() / h + 1);
	}
	
	for (unsigned int i(0); i < nb_excitatory_; ++i) {
		neurons_[i]->setJ(0.1);
	}
	
	for (unsigned int i(nb_excitatory_); i < nb_neurons_; ++i) {
		neurons_[i]->setJ(0.5);
	}
}

void Cortex::initConnections() {
	for (unsigned int i(0); i < nb_neurons_;++i) {
		connections_.push_back({});   //create an empty vector
		for (unsigned int j(0); j < nb_neurons_; ++i) {
			connections_[i].push_back(0);
		}
	}
	
	for (unsigned int i(0); i < nb_excitatory_; ++i) {
		for (unsigned int k(0); k < nb_connections_exc_; ++k) { 
			int rand = random_uniform(nb_neurons_);
			++connections_[i][rand];
		}
	}
	
	for (unsigned int i(nb_excitatory_); i < nb_neurons_; ++i) {
		for (unsigned int k(0); k < nb_connections_inhib_; ++k) { 
			int rand = random_uniform(nb_neurons_);
			++connections_[i][rand];
		}
	}
}

//update all the neurons in the cortex
void Cortex::updateNeurons(std::ofstream & output, double h, long step) {
	for (size_t i(0); i < neurons_.size(); ++i) {
			
		output << "Data for neuron : " << i+1;
		bool spike(neurons_[i]->update(output, h, step)); //update the neuron i
			
		if (spike and (!connections_.empty())) { //if the neuron i had a spike
			for (size_t j(0); j < nb_neurons_; ++j) {
				size_t s = neurons_[j]->getBuffer().size(); //calculate the size of the buffer
				const auto W = (step + s-1) % s; //where we Write in the buffer
				assert(W < s);
				neurons_[i+1]->setBuffer(W, connections_[i][j]); //the neuron i+1 stores J in his buffer
			}
		}
	}
}	
	
void Cortex::printTimeSpikes() {
	for (size_t i(0); i < neurons_.size(); ++i) {
		std::cout << " Times when the spikes occured for neuron " << i+1 << " : " << std::endl;
		for (auto time : neurons_[i]->getTimeSpikes()) {
			std::cout << time << std::endl;
		}
	}
}

void Cortex::setNeuronInput(size_t i, double input) {
	neurons_[i]->setInput(input);
}

int Cortex::random_uniform(unsigned int n) {
	std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, n-1);
    
    return dis(gen);
}	
	
	
