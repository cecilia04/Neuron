#include "neuron.hpp"
#include "cortex.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

int main()
{
	double t_start(0.0);
	double t_stop(100);
	const double h = 0.1; /*! integration step size */
	double I = 0; /*! input when not choosen by the user */
	
	double a, b;
	
	/** the user chooses the time interval when I≠0 */
	do {
		std::cout << "Enter an interval [a,b) in ms with a≥0, b>a and b<500 " << std::endl;
		std::cin >> a >> b;
	} while (a<0 or b<=a or b>500);
	
	/**the user chooses his external input */
	double ext_input;
	std::cout << "Enter your input current in mV " << std::endl;
	std::cin >> ext_input;
	
	long steps_to_a = (a - t_start)/h;
	long steps_to_b = (b - t_start)/h;
	long steps_to_stop = (t_stop - t_start)/h;
	
	long step(0); /*! step we are in */
	
	/** open the file where we are going to write the neurons membrane potential
	 * for each step
	 */
	std::ofstream output;
	output.open("Neuron.txt");
	
	if (output.fail()) {
		std::cout << "Error ! The file could be not open" << std::endl;
	} else {
		
		/** We create our neurons, connect them together and update them */
		Cortex cortex;
		cortex.initNeurons(0, h);
		cortex.initConnections();
		
		while (step < steps_to_a) {
			cortex.setNeuronInput(0, I);
			cortex.setNeuronInput(1, I);
			cortex.updateNeurons(output, h, step); /*! when between t_start and a */
			++step;
		}
		
		while (step < steps_to_b) {
			cortex.setNeuronInput(0, ext_input);
			cortex.setNeuronInput(1, I);
			cortex.updateNeurons(output, h, step); /*! when between a and b */
			++step;
		}
		
		while (step < steps_to_stop) {
			cortex.setNeuronInput(0, I);
			cortex.setNeuronInput(1, I);
			cortex.updateNeurons(output, h, step); /*! when between b and t_stop */
			++step;		
		}
		
		/** Displays when spikes occured for each neuron */
		cortex.printTimeSpikes();
	
	}	
	
	output.close();
}
