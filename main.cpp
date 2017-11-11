#include "experiment.hpp"
#include <iostream>
#include <fstream>
#include <random>

/** \mainpage Description of the program
 * 
 * This program simulates a cortex of 12500 neurons.
 * 12 000 neurons are excitatory and 2 500 are inhibitory.
 * Each neurons have 1 000 excitatory and 250 inhibitory connections
 * 
 * The program will run 4 simulations with different parameters in order
 * to draw plots. 
 * In order to draw the plot for the simulation i:
 * Run the command in the terminal : python graphi.py
 * 
 * Data for the simulation i are available in the fil Expi.csv
 */

int main() {
	const double h = 0.1;  /*! Size of the steps of simulation */
	Experiment exp;
	
//-----------------------------FIRST SIMULATION------------------------------

	std::ofstream output_1;
	output_1.open("Exp1.csv");
	
	if (output_1.fail()) {
		std::cout << "Error ! The file Exp1 could not be open" << std::endl;
	} else {
		exp.simulation(3, 2, h, 600, output_1);
	}
	
	output_1.close();
	
	std::cout << "Experiment 1 done" << std::endl;

//-----------------------------SECOND SIMULATION-----------------------------
	
	std::ofstream output_2;
	output_2.open("Exp2.csv");
	
	if (output_2.fail()) {
		std::cout << "Error ! The file Exp2 could not be open" << std::endl;
	} else {
		exp.simulation(6, 4, h, 1200, output_2);
	}
	
	output_2.close();
	
	std::cout << "Experiment 2 done" << std::endl;
	
//-----------------------------THIRD SIMULATION------------------------------
	
	std::ofstream output_3;
	output_3.open("Exp3.csv");
	
	if (output_3.fail()) {
		std::cout << "Error ! The file Exp3 could not be open" << std::endl;
	} else {
		exp.simulation(5, 2, h, 1200, output_3);
	}
	
	output_3.close();
	
	std::cout << "Experiment 3 done" << std::endl;

//-----------------------------FOURTH SIMULATION------------------------------

	std::ofstream output_4;
	output_4.open("Exp4.csv");
	
	if (output_4.fail()) {
		std::cout << "Error ! The file Exp4 could not be open" << std::endl;
	} else {
		exp.simulation(4.5, 0.9, h, 1200, output_4);
	}
	
	output_4.close();
	
	std::cout << "Experiment 4 done" << std::endl;
}
