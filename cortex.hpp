#ifndef CORTEX_HPP
#define CORTEX_HPP
#include <iostream>
#include <vector>
#include "Neuron.hpp"
#include <fstream>

class Cortex
{
	private:
	
	const unsigned int nb_neurons_ = 2;
	std::vector<Neuron*> neurons_;
	const double J = 0.1;
	
	public:
	
	Cortex(); //constructor
	
	~Cortex(); //destructor
	
	Cortex(Cortex const& another); //copy constructor
	
	void initNeurons(double time, double h); //initialization of the neurons in the cortex
	
	void updateNeurons(double ext_input, std::ofstream & output, double h, long step); //update all the neurons in the cortex
	
	void printTimeSpikes();
	
};

#endif
