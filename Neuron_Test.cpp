#include "gtest/gtest.h"
#include "neuron.hpp"
#include "cortex.hpp"
#include <iostream>
#include <fstream>
#include <cmath>

TEST(OneNeuron, PositiveInput) { //test if computation of the membrane potential is right with a positive input
	Neuron neuron;
	neuron.setInput(1.0);
	neuron.resizeBuffer(1); //just put a random size because the buffer is not used here as we have only 1 neuron
	
	//test for one update with h=0.1
	neuron.update(0.1, 1);
	EXPECT_EQ(20*(1-exp(-0.1/20)) - neuron.getPotential(), 0.0);
	
	//test for numerous updates
	for (long i(0); i < 2000; ++i) {
		neuron.update(0.1, i);
	}
	
	//potential should tend to 20 but never reach it and the neuron should never spike
	EXPECT_LT(neuron.getPotential(), 20.0); //EXPECT_LT -> val1 < val2
	EXPECT_EQ(neuron.getTimeSpikes().size(), 0);
	
	//membrane potential should tend towards 0 with no input current
	neuron.setInput(0.0);
	for (long i(0); i < 2000; ++i) {
		neuron.update(0.1, i);
	}
	EXPECT_NEAR(neuron.getPotential(), 0, 1e-3);
	
}

TEST(OneNeuron, NegativeInput) { //test if computation of the membrane potential is right with a negative input
	Neuron neuron;
	neuron.setInput(-1.0);
	neuron.resizeBuffer(1); //just put a random size because the buffer is not used here as we have only 1 neuron
	
	//test for one update with h=0.1
	neuron.update(0.1, 1);
	EXPECT_EQ(-20*(1-exp(-0.1/20)) - neuron.getPotential(), 0.0);
	
	//test for numerous updates
	for (long i(0); i < 2000; ++i) {
		neuron.update(0.1, i);
	}
	
	//potential should tend to -20 but never reach it and the neuron should never spike
	EXPECT_GT(neuron.getPotential(), -20.0); //EXPECT_GT -> val1 > val2
	EXPECT_EQ(neuron.getTimeSpikes().size(), 0);
	
}


TEST(OneNeuron, SpikeTimes) {
	Neuron neuron;
	neuron.setInput(1.01); //we know that with an input of 1.01mV, spikes occur at 92.4ms, 186.8ms and 281.2ms
	neuron.resizeBuffer(1); //just put a random size because the buffer is not used here as we have only 1 neuron
	
	//Checking first spike
	for (long i(0); i < 924; ++i) {
		neuron.update(0.1, i);
	}
	EXPECT_EQ(neuron.getTimeSpikes().size(), 0);
	neuron.update(0.1, 924);
	EXPECT_EQ(neuron.getTimeSpikes().size(), 1);
	EXPECT_EQ(neuron.getPotential(), 0.0);
	
	//Checking second spike
	for (long i(925); i < 1868; ++i) {
		neuron.update(0.1, i);
	}
	EXPECT_EQ(neuron.getTimeSpikes().size(), 1);
	neuron.update(0.1, 1868);
	EXPECT_EQ(neuron.getTimeSpikes().size(), 2);
	EXPECT_EQ(neuron.getPotential(), 0.0);
	
	//Checking second spike
	for (long i(1869); i < 2812; ++i) {
		neuron.update(0.1, i);
	}
	EXPECT_EQ(neuron.getTimeSpikes().size(), 2);
	neuron.update(0.1, 2312);
	EXPECT_EQ(neuron.getTimeSpikes().size(), 3);
	EXPECT_EQ(neuron.getPotential(), 0.0);
}

TEST(OneNeuron, Simulation) {
	Neuron neuron;
	neuron.setInput(1.01); //we know that with an input of 1.01mV, spikes occur at 92.4ms, 186.8ms and 281.2ms
	neuron.resizeBuffer(1); //just put a random size because the buffer is not used here as we have only 1 neuron
	
	for (long i(0); i < 2815; ++i) {
		neuron.update(0.1, i);
	}
	EXPECT_EQ(neuron.getTimeSpikes().size(), 3);
	EXPECT_EQ(neuron.getTimeSpikes()[0] - 92.4, 0);
	EXPECT_EQ(neuron.getTimeSpikes()[1] - 186.8, 0);
	EXPECT_EQ(neuron.getTimeSpikes()[2] - 281.2,0);
}

TEST(TwoNeurons, N1Spike) {
	Neuron neuron1, neuron2;
	neuron1.setInput(1.01);
	neuron1.resizeBuffer(neuron1.delay_/0.1 + 1);
	neuron2.resizeBuffer(neuron2.delay_/0.1 + 1);
	neuron1.setJ(0.1); //we consider that both neurons are excitatory for the test
	neuron2.setJ(0.1);
	
	//See the impact on neuron 2 when neuron 1 spikes
	for (long i(0); i < 940; ++i) { //number of steps for neuron 1 to spike (924 steps) + delay (15 steps)
		if (neuron1.update(0.1, i)) { //update neuron 1
			size_t s = neuron2.getBuffer().size();
			neuron2.setBuffer((i + s-1) % s); //neuron 2 stores J in his buffer
			EXPECT_EQ(neuron1.getPotential(), 0.0);
		}
		neuron2.update(0.1, i);
	}
	
	EXPECT_EQ(neuron2.getPotential(), 0.1);
}

TEST(TwoNeurons, N2Spike) {
	Neuron neuron1, neuron2;
	neuron1.setInput(1.01);
	neuron2.setInput(1.00);
	neuron1.resizeBuffer(neuron1.delay_/0.1 + 1);
	neuron2.resizeBuffer(neuron2.delay_/0.1 + 1);
	neuron1.setJ(0.1); //we consider that both neurons are excitatory for the test
	neuron2.setJ(0.1);
	
	//Neuron 2 1st spike should occur right after neuron 1 2nd spike
	for (long i(0); i < 1884; ++i) { //number of steps for neuron 1 to spike twice (1868 steps) + delay (15 steps)
		if (neuron1.update(0.1, i)) { //update neuron 1
			size_t s = neuron2.getBuffer().size();
			neuron2.setBuffer((i + s-1) % s); //neuron 2 stores J in his buffer
			EXPECT_EQ(neuron1.getPotential(), 0.0);
		}
		neuron2.update(0.1, i);
	}
	
	//neuron 2 has no spike yet
	EXPECT_EQ(neuron2.getTimeSpikes().size(), 0);
	neuron2.update(0.1, 1884);
	//neuron 2 spike
	EXPECT_EQ(neuron2.getPotential(), 0.0);
	EXPECT_EQ(neuron2.getTimeSpikes().size(), 1);
}

TEST(Cortex_Test, Connections) {
	Cortex cortex;
	cortex.initNeurons(0, 0.1);
	
	EXPECT_EQ(cortex.neurons_.size(), cortex.nb_neurons_);

	cortex.initConnections();
	EXPECT_EQ(cortex.connections_.size(), cortex.nb_neurons_); 
	for (long i(0); i < cortex.nb_neurons_; ++i) {
		EXPECT_EQ(cortex.connections_[i].size(), cortex.nb_connections_exc_ + cortex.nb_connections_inhib_);
	}
}

TEST(Cortex_Test, UniformDistribution) { //test if the random number is always between 0 and nb_neurons - 1
	Cortex cortex;
	for (long i(0); i<12500; ++i) {
		EXPECT_LE(0, cortex.random_uniform(0, cortex.nb_neurons_));
		EXPECT_LE(cortex.random_uniform(0, cortex.nb_neurons_), cortex.nb_neurons_ - 1);
	}
}

int main (int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
	
