#ifndef EXPERIMENT_HPP
#define EXPERIMENT_HPP
#include "cortex.hpp"
#include <iostream>

class Experiment 
{
	private:
	
	Cortex cortex_;
	
	public:
	
	Experiment();
	
	~Experiment();
	
	Experiment(Experiment const& another);
	
	void simulation(double g, double eta, double h, double time, std::ofstream & file_graph);
};

#endif
