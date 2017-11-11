#pragma once

#include <random>
#include <cmath>
#include <vector>
#include <iostream>
#include "classes.h"
#define Pi 3.1416


void calc_R(species &pop, double Intensity[], double alphas[])
{
	for (int q = 0; q < pop.pixels - 2; q++)
		pop.weight[q] = 0;

	pop.best = 0;		// Reset index for best fit

	for (int meeple = 0; meeple < pop.population; ++meeple)
	{
		double prev = 0;
		double value = 0;
		double total = 0;

		for (int p = 1; p < pop.pixels - 1; p++)
		{
			int index = pop.pixels* meeple + p;

			double y = sqrt(Intensity[p + 1]) * sin(pop.arr[index] - pop.arr[index + 1])
					- sqrt(Intensity[p - 1]) * sin(pop.arr[index - 1] - pop.arr[index]);

			double x = sqrt(Intensity[p + 1]) * cos(pop.arr[index] - pop.arr[index + 1])
					- sqrt(Intensity[p - 1]) * cos(pop.arr[index - 1] - pop.arr[index]);

			value = atan2(y, x);
			
			while (prev - value > Pi)
				value += 2 * Pi;

			while (prev - value < -Pi)
				value -= 2 * Pi;

			prev = value;

			double LSE = Intensity[p] * pow(alphas[p] - value, 2);

			pop.weight[p - 1] += LSE;

			total += LSE;
		}

		pop.Rs[meeple] = total;

		if (pop.Rs[meeple] < pop.Rs[pop.best])
				pop.best = meeple;
	}
	
}