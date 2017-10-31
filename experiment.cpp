#include "experiment.hpp"
#include <iostream>

Experiment::Experiment() {
	std::cout << "Making connections ..." << std::endl;
	cortex_.initConnections();
}

Experiment::~Experiment() {}

Experiment::Experiment(Experiment const& another)
	:cortex_(another.cortex_) {}
	
void Experiment::simulation(double g, double eta, double h, double time, std::ofstream & file_graph) {
	cortex_.setG(g);
	cortex_.setEta(eta);
	
	std::cout << "Initializing neurons ..." << std::endl;
	cortex_.initNeurons(0, h);
	
	long step_start = 0;
	long step_stop = time / h;
	
	std::cout << "Updating neurons ..." << std::endl;
	cortex_.updateNeurons(h, step_start, step_stop);
	
	cortex_.saveToFile(file_graph);
	std::cout << "File created !" << std::endl;
	
	cortex_.deleteNeurons();
}
		
	
	


