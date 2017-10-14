#ifndef NEURON_HPP
#define NEURON_HPP
#include <iostream>
#include <vector>

class Neuron
{
	private:
	
	double potential_; 
	unsigned int nb_spikes_;
	const double resistance_ = 20.0;
	const double capacity_ = 1.0;
	const double tau_ = 20.0;
	const double threshold_ = 20.0;
	double refractory_time_ = 2;
	long refractory_steps_ = 0.0; //steps per refractory period
	
	std::vector<double> time_spikes_; //stores the time when a spike occures
	
	double clock_ = 0.0;
	
	double n_J_ = 0.0; //J from other neurons
	
	
	double c1_; //integration constant
	
	public:
	
	Neuron(); //constructor
	
	~Neuron(); //destructor
	
	Neuron(const Neuron& another); //copy constructor
	
	//getters
	double getPotential() const;
	
	unsigned int getNbSpikes() const;
	
	std::vector<double> getTimeSpikes() const;
	
	//setters
	void setClock(double time);
	
	//other functions
	bool update(double ext_input, std::ofstream & output, double h);
	
	void sumInput(double J);
	
	void saveToFile();
};

#endif
