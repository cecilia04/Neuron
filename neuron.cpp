#include "neuron.hpp"
#include <iostream>

Neuron::Neuron() //constructor
	: potential_(0), nb_spikes_(0), resistance_(20), capacity_(1), threshold_(20), refractory_time_(0) {};
	
Neuron::Neuron(const Neuron& another) //destructor
	: potential_(another.potential_), nb_spikes_(another.potential_), time_spikes_(another.time_spikes_) {};
	
Neuron::~Neuron() {}; //copy constructor

//getters 

double Neuron::getPotential() {
	return potential_;
}

unsigned int Neuron::getNb_spikes() {
	return nb_spikes_;
}

std::vector<double> Neuron::getTime_spikes() {
	return time_spikes_;
}

double Neuron::getRefractory_time() {
	return refractory_time_;
}

double Neuron::getCapacity() {
	return capacity_;
}

double Neuron::getResistance() {
	return resistance_;
}

//setters
void Neuron::setPotential(double potential) {
	potential_ = potential;
}

void Neuron::setRefractory_time(double time) {
	refractory_time_ -= time;
}

void Neuron::setResistance(double resistance) {
	resistance_ = resistance;
}

//other functions	
bool Neuron::isRefractory() { //ttesting refractory state (if refractory time has passed)
	return (refractory_time_ > 0.0000); 
}

void Neuron::update(double time, double ext_input) { //update neuron state
	
	if (potential_ > threshold_) {
		
		std::cout << "Potential > threshold" << std::endl;

		refractory_time_ = 2; //potential has reached threshold so neuron is refractory
			
		time_spikes_.push_back(time); //store spike time
			
		++nb_spikes_; //count up new spike
			
		std::cout << "New spike ! We now have " << nb_spikes_ << " spikes." << std::endl;
			
	} else {
			std::cout << "Potential < threshold" << std::endl;
	}
}
		
