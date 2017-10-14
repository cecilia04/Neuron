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
	double delay_ = 2;
	
	std::vector<double> ring_buffer_;
	
	double c1_; //integration constant
	
	public:
	
	Neuron(); //constructor
	
	~Neuron(); //destructor
	
	Neuron(const Neuron& another); //copy constructor
	
	//getters
	double getPotential() const;
	
	unsigned int getNbSpikes() const;
	
	std::vector<double> getTimeSpikes() const;
	
	double getDelay() const;
	
	std::vector<double> getBuffer() const;
	
	//setters
	void setClock(double time);
	
	void setBuffer(size_t i, double J);
	
	//other functions
	bool update(double ext_input, std::ofstream & output, double h, long step);
	
	void saveToFile();
};

#endif
