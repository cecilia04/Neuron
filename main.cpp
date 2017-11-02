#include "experiment.hpp"
#include <iostream>
#include <fstream>

int main() {
	const double h = 0.1;
	Experiment exp;
	
	std::ofstream output;
	output.open("Exp.csv");
	
	if (output.fail()) {
		std::cout << "Error ! The file Exp could not be open" << std::endl;
	} else {
		exp.simulation(5, 2, h, 1200, output);
	}
	
	output.close();
}
