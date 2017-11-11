#include "experiment.hpp"
#include <iostream>

Experiment::Experiment() {}

Experiment::~Experiment() {}

Experiment::Experiment(Experiment const& another)
	:cortex_(another.cortex_) {}
	
	
/** This function runs an entire simulation to create the datas for one graph
 * @param g =Je/Ji
 * @param eta =nu_ext/nu_thr
 * @param h size of the steps of simulation
 * @param time how long must last the simulation
 * @param file_graph file where we write when spikes occured for each neuron
 */
void Experiment::simulation(double g, double eta, double h, double time, std::ofstream & file_graph) {
	std::cout << "Initializing neurons ..." << std::endl;
	cortex_.initNeurons(0, h, g, eta);
	
	std::cout << "Making connections ..." << std::endl;
	cortex_.initConnections();
	
	long step_start = 0;
	long step_stop = time / h;
	
	std::cout << "Updating neurons ..." << std::endl;
	cortex_.updateNeurons(h, step_start, step_stop);
	
	cortex_.saveToFile(file_graph);
	std::cout << "File created !" << std::endl;
	
	cortex_.deleteNeurons();
}
		
	
	


