#pragma once

#include <random>
#include <functional>
#include "classes.h"
#define Pi 3.1416

std::random_device rd;
std::default_random_engine generator(rd());
/*
int rand_pixel(double arr[], double total)
{
	std::uniform_real_distribution<double> pixels(0, total);
	double value = pixels(generator);
	int p = 0;

	while(value >= 0)
	{
		value -= arr[p];
		p++;
	}

	return p;
}
*/
void mutate(species &pop, int count)
{

	/*
	Introduces mutations into population by adding a random phase between 0 and Pi to
	individual pixels of random chromosomes. All values are wrapped back into range
	0 to 2Pi to reduce search area.
	*/

	double weight_total = 0;
	for (int p = 0; p < pop.pixels; p++)
		weight_total += pop.weight[p];

	std::uniform_real_distribution<double> distribution(0, 1);
	std::uniform_real_distribution<double> phase(0, 2*Pi);
	std::uniform_int_distribution<int> pixels(0, pop.pixels);

	auto dart = std::bind(distribution, generator);
	auto rand_phase = std::bind(phase, generator);

	double mutation_rate = 0.1;

	for (int p = 0; p < pop.population; ++p)
	{
		if (p == pop.best)
			continue;
		
		// Mutate from random pixel to end of chromosome
		if (dart() < mutation_rate)
		{
			double rand = rand_phase();
			int pick = pixels(generator);//rand_pixel(pop.weight, weight_total);

			for (int b = pick; b < pop.pixels; ++b)
			{
				pop.arr[p*pop.pixels + b] += rand;
				pop.arr[p*pop.pixels + b] = fmod(pop.arr[p*pop.pixels + b], 2 * Pi);
			}
		}

		// Mutate from beginning of chromosme to random pixel
		if (dart() < mutation_rate)
		{
			double rand = rand_phase();
			int pick = pixels(generator);//rand_pixel(pop.weight, weight_total);
			for (int b = pick; b >= 0; --b)
			{
				pop.arr[p*pop.pixels + b] += rand;
				pop.arr[p*pop.pixels + b] = fmod(pop.arr[p*pop.pixels + b], 2 * Pi);
			}
		}

		// Single Point mutation
		if (dart() < mutation_rate)
		{
			int b = pixels(generator);//rand_pixel(pop.weight, weight_total);
			pop.arr[p*pop.pixels + b] += rand_phase();
			pop.arr[p*pop.pixels + b] = fmod(pop.arr[p*pop.pixels + b], 2 * Pi);
		}

		// Completely random mutation of entire chromosome
		if (dart() < mutation_rate)
			for (int b = 0; b < pop.pixels; ++b)
			{
				pop.arr[p*pop.pixels + b] += rand_phase();
				pop.arr[p*pop.pixels + b] = fmod(pop.arr[p*pop.pixels + b], 2 * Pi);
			}
	}
}