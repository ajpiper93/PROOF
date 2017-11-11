#pragma once

#include <vector>
#include <cmath>
#define Pi 3.1416

class species {
public:
	int pixels, population, length;
	double *arr;
	double *Rs;
	bool *elites;
	double * weight;
	
	species(int pix, int pop)
	{
		pixels = pix; population = pop; length = pix*pop;
		arr = new double[pix*pop];
		Rs = new double[pop];
		elites = new bool[pop];
		weight = new double[pix - 2];				// -2 from fact that two less alphas than pixel
	}

	int best;

	void elitify(double Fitness[])									
	{
		///// Finds top 5% of population to clone (elites) /////
		for (int i = 0; i < population; ++i)
			elites[i] = false;

		int number = int(population / 20);

		double running_max = 0;
		double previous_max = 1000000000000;
		int meeple = 0;

		for (int n = 0; n < number; n++)
		{
			for (int p = 0; p < population; p++)
				if (Fitness[p] > running_max && Fitness[p] < previous_max)
				{
					running_max = Fitness[p];
					meeple = p;
				}

			previous_max = running_max;
			running_max = 0;
			elites[meeple] = true;
		}
	}
	void cataclysm()
	{
		///// Adds new population, excluding elites  //////
		std::random_device rd;
		std::default_random_engine generator(rd());
		std::uniform_real_distribution<double> phase(0, 2 * Pi);

		for (int p = 0; p < population; p++)
		{
			if (elites[p])
				continue;

			for (int q = 0; q < pixels; q++)
			{
				int index = p*pixels + q;
				arr[index] += phase(generator);
				arr[index] = fmod(arr[index], 2 * Pi);
			}
		}
	}

	void initialize()
	{
		std::random_device rd;
		std::default_random_engine generator(rd());
		std::uniform_real_distribution<double> phase(0, 2 * Pi);
		std::uniform_int_distribution<int> integer(0, pixels);

		///// Initialize Population with polynomials of random order and coefficients  //////
		for (int p = 0; p < population; p++)
		{
			int order = integer(generator);
			double* coeff = new double[pixels];

			for (int i = 0; i < order; i++)
				coeff[i] = phase(generator);
			for (int q = 0; q < pixels; q++)
			{
				arr[p*pixels + q] = 0;
				for (int n = 0; n < order; n++)
				{
					arr[p*pixels + q] += coeff[n] * pow((q - population) / 10, n);
					arr[p*pixels + q] = fmod(arr[p*pixels + q], 2 * Pi);
				}
			}

			delete[] coeff;
		}
	}
};
	//~species() { delete[] arr; }
/*
class data {
public:

};
*/