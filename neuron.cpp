#include "neuron.hpp"
#include <iostream>
#include <cmath>
#include <fstream>
#include <cassert>
#include <random>

Neuron::Neuron()
	: potential_(0), nb_spikes_(0) {}
	
Neuron::Neuron(const Neuron& another)
	: potential_(another.potential_), nb_spikes_(another.potential_), time_spikes_(another.time_spikes_) {}
	
Neuron::~Neuron() {}

//---------------------------GETTERS------------------------------

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

long Neuron::getRefractorySteps() const {
	return refractory_steps_;
}	

double Neuron::getClock() const {
	return clock_;
}

std::vector<unsigned int> Neuron::getTargets() const {
	return targets_;
}

double Neuron::getJ() const {
	return J_;
}

double Neuron::getNuExt() const {
	return nu_ext_;
}

//---------------------------SETTERS-------------------------------

void Neuron::setClock(double time) {
	clock_ = time;
}

void Neuron::setBuffer(size_t i, double J) {
	ring_buffer_[i] += J;
}

void Neuron::setInput(double I) {
	Iext_ = I;
}

void Neuron::setJ(double J) {
	J_ = J;
}

void Neuron::setG(double g) {
	g_ = g;
}
	
void Neuron::setEta(double eta) {
	eta_ = eta;
}

//-----------------------OTHER FUNCTIONS----------------------------

/** Updates neuron state
 * @param h timestep size
 * @param step number of the step we are in
 * @param poisson background noise, random with a poisson distribution
 * @return spike boolean if the neuron spiked or not
 */
bool Neuron::update(double h, long step, int poisson) {
	bool spike = false;
		
	if (refractory_steps_ > 0) {--refractory_steps_;} /*! updates the neuron refractory period */
		 
	if (potential_ >= threshold_) {		/*!If the potential reaches the threshold */

		refractory_steps_ = refractory_time_ / h; /*! neuron is refractory */
			
		time_spikes_.push_back(clock_); /*! stores spike time */
			
		++nb_spikes_; /*! counts up new spike */
			
		spike = true;
			
	}
	
	const auto R = step % (ring_buffer_.size()); /*! computes where we Read in our buffer */
	assert(R < ring_buffer_.size());
		
	if (isRefractory()) { 	/*! if the neuron is refractory, potential is 0 */
		potential_ = 0.0;
	} else { 				/*! otherwise we update the potential following the formula */
		assert(spike == 0);
		potential_ = c1_ * potential_ + Iext_ * c2_ + ring_buffer_[R] + 0.1 * poisson;
	}
	
	ring_buffer_[R] = 0.0;
	
	clock_ = (step+1)*h; /*! updates the neuron clock */
	return spike;
}

/** Resizes the ring buffer of the neuron
 * @param i size we want the buffer to have
 */
void Neuron::resizeBuffer(int i) {
	ring_buffer_.resize(i);
}

/** Computes c1_, c2_ and nu_ext_ */
void Neuron::computeConstants(double h) {
	c1_ = exp(-h/tau_);
	c2_ = resistance_ * (1-c1_);
	nu_ext_ = eta_ * threshold_ / (0.1 * tau_);
}

bool Neuron::isRefractory() {
	return (refractory_steps_ > 0);
}

/** Fills the targets vector with the neuron i */
void Neuron::fillTargets(unsigned int i) {
	targets_.push_back(i);
}
