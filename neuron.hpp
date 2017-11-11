#ifndef NEURON_HPP
#define NEURON_HPP
#include <iostream>
#include <vector>

class Neuron
{
	private:
	
		#ifdef TEST
	FRIEND_TEST (TwoNeurons,N1Spike);
	FRIEND_TEST (TwoNeurons, N2Spike); 
		#endif
	
	double potential_; 
	unsigned int nb_spikes_;
	const double resistance_ = 20.0; //because capacity_ = 1.0 and  and resistance = tau / capacity
	const double tau_ = 20.0;
	const double threshold_ = 20.0;
	double refractory_time_ = 2;
	long refractory_steps_ = 0.0; // steps per refractory period
	
	double g_ = 5;
	double eta_ = 2; // nu ext / nu thr
	
	
	double Iext_ = 0;
	double J_ = 0.1;
	
	double nu_ext_;

	//integration constants
	double c1_; 
	double c2_;
	
	std::vector<double> time_spikes_; // stores the time when a spike occures
	
	double clock_ = 0.0;
	double delay_ = 1.5;
	
	std::vector<double> ring_buffer_;
	std::vector<unsigned int> targets_;
	
	public:
	
	Neuron();
	
	~Neuron();
	
	Neuron(const Neuron& another);
	
	//getters
	double getPotential() const;
	
	unsigned int getNbSpikes() const;
	
	std::vector<double> getTimeSpikes() const;
	
	double getDelay() const;
	
	std::vector<double> getBuffer() const;
	
	long getRefractorySteps() const;
	
	double getClock() const;
	
	std::vector<unsigned int> getTargets() const;

	double getJ() const;
	
	double getNuExt() const;
	
	//setters
	void setClock(double time);
	
	void setBuffer(size_t i, double J);
	
	void setInput(double I);
	
	void setJ(double J);
	
	void setG(double g);
	
	void setEta(double eta);
	
	//other functions
	bool update(double h, long step, int poisson);
	
	void resizeBuffer(int i);
	
	void computeConstants(double h);
	
	bool isRefractory();
	
	void fillTargets(unsigned int i);
	
};

#endif
