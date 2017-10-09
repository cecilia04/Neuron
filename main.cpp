#include "neuron.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

int main()
{
	double t_start(0.0);
	double t_stop(400);
	
	double a, b;
	
	//the user chooses the time interval when Iext≠0
	do {
		std::cout << "Enter an interval [a,b) in ms with a≥0 and b>a " << std::endl;
		std::cin >> a >> b;
	} while (a<0 or b<=a);
	
	//the user chooses his external input
	double ext_input;
	std::cout << "Enter your input current in mV " << std::endl;
	std::cin >> ext_input;
	
	double dt(0.1); //h = simulation step
	
		
	
	double simtime(t_start);
	
	std::ofstream output;
	output.open("Neuron.txt");
	
	if (output.fail()) {
		std::cout << "Error ! The file could not open" << std::endl;
	} else {

		Neuron neuron;
		double Iext; //Iext that we are going to use
	
		while(simtime < t_stop) {
			std::cout << "Simtime: " << simtime << " ms" << std::endl;
			
			if (a <= simtime and simtime < b) { //choose the correct input to use for this simulation
				Iext = ext_input;
			} else {
				Iext = 0;
			}
			
			neuron.setRefractory_time(dt); //substract the simulation step from refractory time, doesn't matter is negative
			std::cout << "Neuron refractory time : " << neuron.getRefractory_time() << std::endl;
			
			if (neuron.isRefractory()) {
				std::cout << "Neuron is refractory" << std::endl; 
				neuron.setPotential(0); // reset potential 
			} else {
				std::cout << "Neuron is not refractory" << std::endl;
				neuron.update(simtime, Iext); //update neuron state
				
				double tau(20); //resistance * capacity
				double const_e( exp(-dt/tau) );
				neuron.setResistance(tau / neuron.getCapacity());
				neuron.setPotential(const_e * neuron.getPotential() + Iext * neuron.getResistance() * (1 - const_e)); //compute new potential
			}
			
			std::cout << "New neuron potential : " << neuron.getPotential() << std::endl;
			
			output << "Time: " << simtime << " ms" << std::endl;
			output << "Membrane potential: " << neuron.getPotential() << std::endl;	
			
			simtime+= dt;
		}
		
		std::cout << "Time when the spikes occured: " << std::endl;
		for (auto element : neuron.getTime_spikes()) {
			std::cout << element << std::endl;
		}
	}
	
	output.close();
}
