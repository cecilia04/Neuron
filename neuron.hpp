#ifndef NEURON_HPP
#define NEURON_HPP
#include <iostream>
#include <vector>

class Neuron
{
	private:
	
	double potential_; 
	unsigned int nb_spikes_;
	double resistance_;
	double capacity_;
	
	double threshold_;
	double refractory_time_;
	
	std::vector<double> time_spikes_;
	
	public:
	
	Neuron(); //constructor
	
	~Neuron(); //destructor
	
	Neuron(const Neuron& another); //copy constructor
	
	//getters

	double getPotential();
	
	unsigned int getNb_spikes();
	
	std::vector<double> getTime_spikes();
	
	double getRefractory_time();
	
	double getCapacity();
	
	double getResistance();
	
	//setters
	void setPotential(double potential);
	
	void setRefractory_time(double time);
	
	void setResistance(double resistance);
	
	//other functions
	void update(double dt, double ext_input);
	
	bool isRefractory();
};

#endif
