#include "generation.h"
#include "calc_R.h"
#include "mutate.h"
#include "array_math.h"
#include "classes.h"

#include <cmath>
#include <fstream>
#include <thread>
#include <string>
#include <vector>
#include <iostream>
#include "windows.h"
#define Pi 3.1416

const int population = 100;						// Keep an even number
const int count = 10000;							// Maximum number of generations
//double emin, eres, efund;
int pixels = 46;								// Number of Harmonics in spectrogram

//////////////////////////////////////////////////////////////////////////////


int main()
{
	/////////////////////////////  Unpack Experimental Data  ///////////////////////////////////////////
	double* alpha = new double[pixels];
	double* intensity = new double[pixels];

	std::ifstream Intensities("C:\\Users\\Admin\\Desktop\\Research\\Proof\\Intensity.txt", std::ios::in);
	if (Intensities.is_open())
	{
		std::string in;
		for (int p = 0; p < pixels; p++)
		{
			getline(Intensities, in);
			intensity[p] = std::stod(in);
		}
	}
	Intensities.close();

	std::ifstream Alphas("C:\\Users\\Admin\\Desktop\\Research\\Proof\\Alpha.txt", std::ios::in);
	if (Alphas.is_open())
	{
		std::string al;
		for (int p = 0; p < pixels; p++)
		{
			getline(Alphas, al);
			alpha[p] = std::stod(al);
		}
	}
	Alphas.close();

	species pop(pixels, population);

	///////////////////////////		Main Function	  ///////////////////////////////////////////
	std::ofstream rs("C:\\Users\\Admin\\Desktop\\Research\\Proof\\Rmins.txt", std::ios::trunc);

	//////////////////////  Populate  ///////////////


	pop.initialize();

	int iter = 0;

	////////  Primary Loop of Program (Evaluate Fitness Functions -> Print data on best fit -> Generate new members of population -> Introduce Mutations)  /////////////////
	do
	{
		//Apply Fitness Function to Population, also updates the index for best fit in population (pop.best)
		calc_R(std::ref(pop), intensity, alpha);			

		//Save Fitness Function Data to File
		rs << pop.Rs[pop.best] << "\t" << avg(pop.Rs, pop.population) << "\t" << sigma(pop.Rs, pop.population) << "\n";		

		//if (iter == count)
			//break;	
		
		if (iter % 100 == 0)
			pop.cataclysm();

		if (pop.Rs[pop.best] < 0.1)
			break;

		generation(std::ref(pop));				// Reproduction (Generate new members of population)

		mutate(std::ref(pop), count);			// Mutate individual chromosomes of population members

		iter++;

	} while (true);

	rs.close();
	///////////////////////////////////////////////////////////////////////

	/////////////////////    Save extracted Phase   //////////////////////////////////////
	std::ofstream save("C:\\Users\\Admin\\Desktop\\Research\\Proof\\bestest0.txt", std::ios::trunc);

	save << pop.Rs[pop.best] << "\n";

	double* best = new double[pop.pixels];

	for (int p = 0; p < pop.pixels; ++p)
		best[p] = pop.arr[pop.best*pop.pixels + p];

	for (int p = 0; p < pixels; ++p)
	{
		double omega = 1.5*p + 10.5;

		save << omega << "\t";
		save << intensity[p] << "\t";
		save << best[p] << "\n";
	}

	save.close();
	////////////////////////////////////////////////////////////////////////////////////////


	///////////////////   Save alphas of extracted phases  /////////////////////////////////
	std::ofstream save2("C:\\Users\\Admin\\Desktop\\Research\\Proof\\alphas1.txt", std::ios::trunc);

	double prev = 0;
	double value = 0;

	for (int p = 1; p < pop.pixels - 1; p++)
	{
		double y = sqrt(intensity[p + 1]) * sin(pop.arr[pop.pixels* pop.best + p] - pop.arr[pop.pixels* pop.best + p + 1])
			- sqrt(intensity[p - 1]) * sin(pop.arr[pop.pixels* pop.best + p - 1] - pop.arr[pop.pixels* pop.best + p]);

		double x = sqrt(intensity[p + 1]) * cos(pop.arr[pop.pixels* pop.best + p] - pop.arr[pop.pixels* pop.best + p + 1])
			- sqrt(intensity[p - 1]) * cos(pop.arr[pop.pixels* pop.best + p - 1] - pop.arr[pop.pixels* pop.best + p]);

		value = atan2(y, x);

		while (prev - value > Pi)
			value += 2 * Pi;

		while (prev - value < -Pi)
			value -= 2 * Pi;

		prev = value;

		save2 << value << '\n';
	}

	delete[] alpha;
	delete[] intensity;
	delete[] best;

	save2.close();
	//////////////////////////////////////////////////////////////////////////

	Beep(523, 500);
	return 0;
}
