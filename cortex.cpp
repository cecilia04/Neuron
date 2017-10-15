#include "cortex.hpp"

Cortex::Cortex() {}; //constructor
	
Cortex::~Cortex() {}; //destructor
	
Cortex::Cortex(Cortex const& another) //copy constructor
	:neurons_(another.neurons_) {};
	
//initialization of the neurons in the cortex
void Cortex::initNeurons(double time, double h) {
	for (unsigned int i(0); i < nb_neurons_; ++i) {
		neurons_.push_back(new Neuron);
		neurons_[i]->setClock(time);
		neurons_[i]->resizeBuffer(neurons_[i]->getDelay() / h);
	}
}

//update all the neurons in the cortex
void Cortex::updateNeurons(double ext_input, std::ofstream & output, double h, long step) {
	for (size_t i(0); i < neurons_.size(); ++i) {
			
			output << "Data for neuron : " << i+1;
			bool spike(neurons_[i]->update(ext_input, output, h, step)); //update the neuron i
			
			if (spike and (i+1) < neurons_.size()) { //if the neuron i had a spike
				size_t s = neurons_[i+1]->getBuffer().size(); //calculate the size of the buffer
				neurons_[i+1]->setBuffer((step + s-1) % s, J); 		//the neuron i+1 stores J in his buffer
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
