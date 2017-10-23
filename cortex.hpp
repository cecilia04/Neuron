#ifndef CORTEX_HPP
#define CORTEX_HPP
#include <iostream>
#include <vector>
#include "neuron.hpp"
#include <fstream>

class Cortex
{
	private:
	
		#ifdef TEST
	FRIEND_TEST (Neuron_Test, SpikeTimes); 
		#endif
	
	const unsigned int nb_neurons_ = 12500;
	const unsigned int nb_excitatory_ = 10000;
	const unsigned int nb_inhibitory_ = 2500;
	const unsigned int nb_connexions_exc_ = 1000;
	const unsigned int nb_connexions_inhib_ = 250;
	std::vector<Neuron*> neurons_;
	
	std::vector<std::vector<int>> connexions_; //matrix to know how many connexions there are between each neurons
	
	public:
	
	Cortex(); //constructor
	
	~Cortex(); //destructor
	
	Cortex(Cortex const& another); //copy constructor
	
	void initNeurons(double time, double h); //initialization of the neurons in the cortex
	
	void initConnexions(); //initialization of connexions between the neurons
	
	void updateNeurons(std::ofstream & output, double h, long step); //update all the neurons in the cortex
	
	void printTimeSpikes();
	
	void setNeuronInput(size_t i, double input);
	
	int random_uniform(unsigned int n); //return a random number between 0 and n with a uniform distribution
	
};

#endif
