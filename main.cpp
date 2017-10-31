#include "experiment.hpp"
#include <iostream>
#include <fstream>

int main() {
	const double h = 0.1; /*! timestep size */
	Experiment exp;
	
	std::cout << "Experiment 1" << std::endl;
	std::ofstream exp1;
	exp1.open("Exp_1.csv");
	
	if (exp1.fail()) {
		std::cout << "Error ! The file Exp1 could not be open" << std::endl;
	} else {
		exp.simulation(3, 2, h, 600, exp1);
	}
	
	exp1.close();
	
	std::cout << "Experiment 2" << std::endl;
	std::ofstream exp2;
	exp2.open("Exp_2.csv");
	
	if (exp2.fail()) {
		std::cout << "Error ! The file Exp2 could not be open" << std::endl;
	} else {
		exp.simulation(6, 4, h, 1200, exp2);
	}
	
	exp2.close();
	
	std::cout << "Experiment 3" << std::endl;
	std::ofstream exp3;
	exp3.open("Exp_3.csv");
	
	if (exp3.fail()) {
		std::cout << "Error ! The file Exp3 could not be open" << std::endl;
	} else {
		exp.simulation(5, 2, h, 1200, exp3);
	}
	
	exp3.close();
	
	std::cout << "Experiment 4" << std::endl;
	std::ofstream exp4;
	exp4.open("Exp_4.csv");
	
	if (exp4.fail()) {
		std::cout << "Error ! The file Exp4 could not be open" << std::endl;
	} else {
		exp.simulation(4.5, 0.9, h, 1200, exp4);
	}
	
	exp1.close();
	
	std::cout << "Experiments done" << std::endl;
}
