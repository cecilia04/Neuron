#include "Cortex.hpp"

Cortex::Cortex() {}; //constructor
	
Cortex::~Cortex() {}; //destructor
	
Cortex::Cortex(Cortex const& another) //copy constructor
	:neurons_(another.neurons_) {};
	
//initialization of the neurons in the cortex
void Cortex::initNeurons(double time) {
	for (unsigned int i(0); i < nb_neurons_; ++i) {
		neurons_.push_back(new Neuron);
		neurons_[i]->setClock(time);
	}
}

//update all the neurons in the cortex
void Cortex::updateNeurons(double ext_input, std::ofstream & output, double h) {
	for (size_t i(0); i < neurons_.size(); ++i) {
			
			output << "Data for neuron : " << i+1;
			bool spike(neurons_[i]->update(ext_input, output, h)); //update the neuron i
			
			if (spike and (i+1) < neurons_.size()) { //if the neuron i had a spike
				neurons_[i+1]->sumInput(J); 		//the neuron i+1 stores J
			}
		}
}	
	
void Cortex::printTimeSpikes() {
	for (size_t i(0); i < neurons_.size(); ++i) {
		std::cout << "Times when the spikes occured for neuron " << i+1 << " : " << std::endl;
		for (auto time : neurons_[i]->getTimeSpikes()) {
			std::cout << time << std::endl;
		}
	}
}			
