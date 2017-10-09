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
	
	double clock_;
	
	double n_J_ = 0; //J from other neurons
	
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
	
	double getN_J();
	
	//setters
	void setPotential(double potential);
	
	void setRefractory_time(double time);
	
	void setResistance(double resistance);
	
	void setClock(double time);
	
	void setN_J(double x);
	
	//other functions
	bool update(double dt, double ext_input);
	
	bool isRefractory();
	
	void sumInput(double J);
};

#endif
