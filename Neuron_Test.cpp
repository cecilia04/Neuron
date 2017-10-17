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
	neuron.update(output, 0.1, 2000);
	
	EXPECT_LT(neuron.getPotential(), 20.0); //potential should tend to 20 but never reach it
	
	output.close();
}

int main (int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
	
