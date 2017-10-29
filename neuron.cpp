#include "neuron.hpp"
#include <iostream>
#include <cmath>
#include <fstream>
#include <cassert>
#include <random>

Neuron::Neuron() /*! constructor */
	: potential_(0), nb_spikes_(0) {}
	
Neuron::Neuron(const Neuron& another) /*! destructor */
	: potential_(another.potential_), nb_spikes_(another.potential_), time_spikes_(another.time_spikes_) {}
	
Neuron::~Neuron() {} /*! copy constructor */

/**getters */
double Neuron::getPotential() const {
	return potential_;
}
	
unsigned int Neuron::getNbSpikes() const {
	return nb_spikes_;
}
	
std::vector<double> Neuron::getTimeSpikes()  const {
	return time_spikes_;
}

double Neuron::getDelay() const {
	return delay_;
}

std::vector<double> Neuron::getBuffer() const {
	return ring_buffer_;
}
	

/**setters */
void Neuron::setClock(double time) {
	clock_ = time;
}

void Neuron::setBuffer(size_t i, int n) {
	ring_buffer_[i] += n * J_;
}

void Neuron::setInput(double I) {
	Iext_ = I;
}

void Neuron::setJ(double J) {
	J_ = J;
}

/**other functions */

/** Updates neuron state
 * @param output the file where we write membrane potential
 * @param h timestep size
 * @param step number of the step we are in
 * @return spike boolean if the neuron spiked or not
 */

bool Neuron::update(std::ofstream & output, double h, long step) {
	
	bool spike = false;
		
	--refractory_steps_; /*! updates the neuron refractory period */
		 
	if (potential_ > threshold_) {

		refractory_steps_ = refractory_time_ / h; /*! potential has reached threshold so neuron is refractory */
			
		time_spikes_.push_back(clock_); /*! stores spike time */
			
		++nb_spikes_; /*! counts up new spike */
			
		spike = true;
			
	}
		
	if (refractory_steps_ > 0) { /*! if the neuron is refractory */
		potential_ = 0.0;
	} else {
		c1_ = exp(-h/tau_);
		const auto R = step % (ring_buffer_.size()); /*! where we Read in our buffer */
		assert(R < ring_buffer_.size());
		potential_ = c1_ * potential_ + Iext_ * resistance_ * (1- c1_) + ring_buffer_[R];
		ring_buffer_[R] = 0.0;
	}
	
	output << "Time : " << clock_ << " ms; Membrane potential : " << potential_ << " mV" << std::endl;
	clock_ = (step+1)*h; /*! updates the neuron clock */
	
	return spike;
}

/** Resizes the ring buffer of the neuron
 * @param i size we want the buffer to have
 */
void Neuron::resizeBuffer(int i) {
	ring_buffer_.resize(i);
}


/** Generates a random int with Poisson distribution */
int Neuron::random_poisson() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::poisson_distribution<> dis(0.2); /*! here we use 0.2 mV/step with 0.02 mV/(connexion*ms) */
	
	return dis(gen);
}
