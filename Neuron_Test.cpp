#include "gtest/gtest.h"
#include "neuron.hpp"
#include "cortex.hpp"
#include <iostream>
#include <fstream>
#include <cmath>

TEST(Neuron_Test, PositiveInput) { //test if computation of the membrane potential is right with a positive input
	Neuron neuron;
	neuron.setInput(1.0);
	neuron.resizeBuffer(1); //just put a random size because the buffer is not used here as we have only 1 neuron
	
	std::ofstream output;
	output.open("Test1.txt"); //the fonction update needs a file but we don't care about that now
	
	//test for one update with h=0.1
	neuron.update(output, 0.1, 1);
	EXPECT_EQ(20*(1-exp(-0.1/20)) - neuron.getPotential(), 0.0);
	
	//test for numerous updates
	for (long i(0); i < 2000; ++i) {
		neuron.update(output, 0.1, i);
	}
	
	//potential should tend to 20 but never reach it and the neuron should never spike
	EXPECT_LT(neuron.getPotential(), 20.0); //EXPECT_LT -> val1 < val2
	EXPECT_EQ(neuron.getTimeSpikes().size(), 0);
	
	//membrane potential should tend towards 0 with no input current
	neuron.setInput(0.0);
	for (long i(0); i < 2000; ++i) {
		neuron.update(output, 0.1, i);
	}
	EXPECT_NEAR(neuron.getPotential(), 0, 1e-3);
	
	output.close();
}

TEST(Neuron_Test, NegativeInput) { //test if computation of the membrane potential is right with a negative input
	Neuron neuron;
	neuron.setInput(-1.0);
	neuron.resizeBuffer(1); //just put a random size because the buffer is not used here as we have only 1 neuron
	
	std::ofstream output;
	output.open("Test1.txt"); //the fonction update needs a file but we don't care about that now
	
	//test for one update with h=0.1
	neuron.update(output, 0.1, 1);
	EXPECT_EQ(-20*(1-exp(-0.1/20)) - neuron.getPotential(), 0.0);
	
	//test for numerous updates
	for (long i(0); i < 2000; ++i) {
		neuron.update(output, 0.1, i);
	}
	
	//potential should tend to -20 but never reach it and the neuron should never spike
	EXPECT_GT(neuron.getPotential(), -20.0); //EXPECT_GT -> val1 > val2
	EXPECT_EQ(neuron.getTimeSpikes().size(), 0);
	
	output.close();
}


TEST(Neuron_Test, SpikeTimes) {
	Neuron neuron;
	neuron.setInput(1.01); //we know that with an input of 1.01mV, spikes occur at 92.4ms, 186.8ms and 281.2ms
	neuron.resizeBuffer(1); //just put a random size because the buffer is not used here as we have only 1 neuron
	
	std::ofstream output;
	output.open("Test1.txt"); //the fonction update needs a file but we don't care about that now
	
	//Checking first spike
	for (long i(0); i < 924; ++i) {
		neuron.update(output, 0.1, i);
	}
	EXPECT_EQ(neuron.getTimeSpikes().size(), 0);
	neuron.update(output, 0.1, 924);
	EXPECT_EQ(neuron.getTimeSpikes().size(), 1);
	EXPECT_EQ(neuron.getPotential(), 0.0);
	
	//Checking second spike
	for (long i(925); i < 1868; ++i) {
		neuron.update(output, 0.1, i);
	}
	EXPECT_EQ(neuron.getTimeSpikes().size(), 1);
	neuron.update(output, 0.1, 1868);
	EXPECT_EQ(neuron.getTimeSpikes().size(), 2);
	EXPECT_EQ(neuron.getPotential(), 0.0);
	
	//Checking second spike
	for (long i(1869); i < 2812; ++i) {
		neuron.update(output, 0.1, i);
	}
	EXPECT_EQ(neuron.getTimeSpikes().size(), 2);
	neuron.update(output, 0.1, 2313);
	EXPECT_EQ(neuron.getTimeSpikes().size(), 3);
	EXPECT_EQ(neuron.getPotential(), 0.0);
	
	output.close();
}

TEST(Neuron_Test, Simulation) {
	Neuron neuron;
	neuron.setInput(1.01); //we know that with an input of 1.01mV, spikes occur at 92.4ms, 186.8ms and 281.2ms
	neuron.resizeBuffer(1); //just put a random size because the buffer is not used here as we have only 1 neuron
	
	std::ofstream output;
	output.open("Test1.txt"); //the fonction update needs a file but we don't care about that now
	
	for (long i(0); i < 2815; ++i) {
		neuron.update(output, 0.1, i);
	}
	EXPECT_EQ(neuron.getTimeSpikes().size(), 3);
	EXPECT_EQ(neuron.getTimeSpikes()[0] - 92.4, 0);
	EXPECT_EQ(neuron.getTimeSpikes()[1] - 186.8, 0);
	EXPECT_EQ(neuron.getTimeSpikes()[2] - 281.2,0);
	
	output.close();
}

int main (int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
	
