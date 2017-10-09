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
	
	int nb_neurons;
	std::cout << "How many neurons do you want ?" << std::endl;
	std::cin >> nb_neurons;
	
	std::vector<Neuron*> neurons;
	for (int i(0); i < nb_neurons; ++i) {
		neurons.push_back(new Neuron);
	}
	
	const double J = 0.1; //amplitude en mV
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
			//std::cout << "Simtime: " << simtime << " ms" << std::endl;
			
			if (a <= simtime and simtime < b) { //choose the correct input to use for this simulation
				Iext = ext_input;
			} else {
				Iext = 0;
			}
			
			for (int i(0); i < neurons.size(); ++i) {
				neurons[i]->setClock(simtime);
				neurons[i]->setRefractory_time(dt); //substract the simulation step from refractory time, doesn't matter is negative
				//std::cout << "Neuron refractory time : " << neurons[i]->getRefractory_time() << std::endl;
			
				if (neurons[i]->isRefractory()) {
					//std::cout << "Neuron is refractory" << std::endl; 
					neurons[i]->setPotential(0.01); // reset potential 
				} else {
					//std::cout << "Neuron is not refractory" << std::endl;
					bool spike(neurons[i]->update(simtime, Iext)); //update neuron state
					//std::cout << "etat du neurone: " << spike << std::endl;
					
					double tau(20); //resistance * capacity
					double const_e( exp(-dt/tau) );
					neurons[i]->setResistance(tau / neurons[i]->getCapacity());
					neurons[i]->setPotential(const_e * neurons[i]->getPotential() + Iext * neurons[i]->getResistance() * (1 - const_e) + neurons[i]->getN_J()); //compute new potential
					neurons[i]->setN_J(0);
					//std::cout << "Potentiel Neurone " << i+1 << " " << neurons[i]->getPotential() << std::endl;

					
					if (spike and (i+1) < neurons.size()) {
						//std::cout << "in the loop" << std::endl;
						neurons[i+1]->sumInput(J);
					}
				
				}
			
				//std::cout << "New neuron potential : " << neurons[i]->getPotential() << std::endl;
			
				output << "Time: " << simtime << " ms" << std::endl;
				output << "Membrane potential: " << neurons[i]->getPotential() << std::endl;	
			}
			
			simtime+= dt;
		}
		
		for (int i(0); i < neurons.size(); ++i) {
			std::cout << "Time when the spikes occured for neuron: " << i+1 << std::endl;
			for (auto element : neurons[i]->getTime_spikes()) {
				std::cout << element << std::endl;
			}	
		}
	}
	
	output.close();
}
