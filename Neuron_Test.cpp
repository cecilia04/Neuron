#include "gtest/gtest.h"
#include "neuron.hpp"
#include "cortex.hpp"
#include <iostream>
#include <fstream>
#include <cmath>

TEST(Neuron_Test, MembranePotential) { //test if computation of the membrane potential is right
	Neuron neuron;
	neuron.setInput(1.0);
	neuron.resizeBuffer(1); //just put a random size because the buffer is not used here as we have only 1 neuron
	
	std::ofstream output;
	output.open("Test1.txt"); //the fonction update needs a file but we don't care about that now
	
	//test for one update with h=0.1
	neuron.update(output, 0.1, 1);
	
	EXPECT_EQ(20*(1-exp(-0.1/20)) - neuron.getPotential(), 0.0);
	
	//test for 2000 updates
	for (long i(0); i < 2000; ++i) {
		neuron.update(output, 0.1, i);
	}
	
	//potential should tend to 20 but never reach it
	EXPECT_LT(neuron.getPotential(), 20.0); //EXPECT_LT -> val1 < val2
	
	output.close();
}

TEST(Neuron_Test, SpikeTimes) {
	Cortex cortex;
	const double h = 0.1;
	cortex.initNeurons(0, h); //2 neurons with clock = 0
	
	std::ofstream output;
	output.open("Test2.txt");
	
	for (long i(0); i < 1000; ++i) {
		cortex.setNeuronInput(0, 0);
		cortex.setNeuronInput(1, 0);
		cortex.updateNeurons(output, h, i);
	}
	
	for (long i(1000); i < 4000; ++i) {
		cortex.setNeuronInput(0, 1.01);    //Ext input = 1.01mV between 100 and 400ms
		cortex.setNeuronInput(1, 0);
		cortex.updateNeurons(output, h, i);
	}
	
	for (long i(3000); i < 5000; ++i) {
		cortex.setNeuronInput(0, 0);
		cortex.setNeuronInput(1, 0);
		cortex.updateNeurons(output, h, i);
	}
		
	output.close();
	
	EXPECT_EQ(cortex.neurons_[0]->time_spikes_.size(), 3); //First neuron should have 3 spikes
	EXPECT_FLOAT_EQ(cortex.neurons_[0]->time_spikes_[0], 192.4);
	EXPECT_FLOAT_EQ(cortex.neurons_[0]->time_spikes_[1], 286.8);
	EXPECT_FLOAT_EQ(cortex.neurons_[0]->time_spikes_[2], 381.2);
	
	EXPECT_EQ(cortex.neurons_[1]->time_spikes_.size(), 0); //Second neuron should not have any spike
}

int main (int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
	
