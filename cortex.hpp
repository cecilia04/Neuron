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
	FRIEND_TEST (Cortex_Test, Connections);
	FRIEND_TEST (Cortex_Test, UniformDistribution);
		#endif
		
	const unsigned int nb_neurons_ = 12500;
	const unsigned int nb_excitatory_ = 10000;   // => nb_inhibitory_ = 2500;
	const unsigned int nb_connections_exc_ = 1000;
	const unsigned int nb_connections_inhib_ = 250;
	
	std::vector<Neuron*> neurons_;
	
	public:
	
	Cortex();
	
	~Cortex();
	
	Cortex(Cortex const& another);
	
	void initNeurons(double time, double h, double g, double eta);
	
	void initConnections();
	
	void updateNeurons(double h, long step_start, long step_stop);
	
	void printTimeSpikes();
	
	void saveToFile(std::ofstream & file);
	
	void setNeuronInput(size_t i, double input);
	
	void deleteNeurons();
	
};

#endif
