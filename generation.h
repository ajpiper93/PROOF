#pragma once
#include "calc_R.h"
#include "array_math.h"
#include "classes.h"

#include <iostream>
#include <random>
#include <functional>
#include <thread>


void generation(species &pop)
{
	/*
	This function generates new members of the population by cloning elites (top 5% of population),
	and random crossovers between members	
	*/
	double* child = new double[pop.length];
	double* Fitness = new double[pop.population];

	////////	Calculate Fitness		//////////////
	for (int r = 0; r < pop.population; r++)
		Fitness[r] = pop.Rs[pop.best] / pop.Rs[r];
	//////////////////////////////////////////////////////	

	pop.elitify(Fitness);			// Identifies top 5% of population for cloning (see class definition in classes.h for code)

	//////////////////////////////////////////////////////////////

	///////////////////////////////////////////////			Mating		 /////////////////////////////////////////////////////
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_int_distribution<int> pop_dist(0, pop.population);
	std::uniform_int_distribution<int> bit_dist(0, pop.pixels);
	std::uniform_real_distribution<double> distribution(0, 1);

	auto dart = std::bind(distribution, generator);
	auto pop_roll = std::bind(pop_dist, generator);
	auto bit_roll = std::bind(bit_dist, generator);

	double Ftot = 0;
	for (int i = 0; i < pop.population; i++)
		Ftot += Fitness[i];

	int parent1, parent2, crossover;

	for (int meeple = 0; meeple < pop.population; ++meeple)					// 4 is number of threads
	{
		///////////////////		Select Parents		////////////////////////////
		double choose = dart();
		double probability = 0;
		parent1 = 1;
		parent2 = 2;
		
		for (int r = 0; r < pop.population; r++)			//// Is there a better way to do this????????
		{
			probability += Fitness[r] / Ftot;
			if (choose < probability)
			{
				parent1 = r;
				break;
			}
		}

		probability = 0;
		for (int r = 0; r < pop.population; r++)
		{
			if (r == parent1)
				continue;

			probability += Fitness[r] / (Ftot - Fitness[parent1]);
			if (choose < probability)
			{
				parent2 = r;
				break;
			}
		}
		
		//////////////////////////////////////////////////////////////////////
		
		//////////////////////////		Crossover		//////////////////////////
		crossover = bit_roll();

		// Clone elites
		if (pop.elites[meeple])
		{
			for (int x = 0; x < pop.pixels; x++)
				child[meeple*pop.pixels + x] = pop.arr[meeple*pop.pixels + x];
			continue;
		}

		
		// Single point crossover, all pixels are crossed between parents to offspring from a random pixel to end of chromosome
		else if (dart() < 0.1)
		{

			for (int x = 0; x < pop.pixels; x++)
			{
				if (x < crossover)
					child[meeple*pop.pixels + x] = pop.arr[parent1*pop.pixels + x];
				else
					child[meeple*pop.pixels + x] = pop.arr[parent2*pop.pixels + x];
			}
		}
		
		else if (dart() < 0.111)
		{
			for (int x = 0; x < pop.pixels; x++)
			{
				if (dart() < 0.5)
					child[meeple*pop.pixels + x] = pop.arr[parent1*pop.pixels + x];
				else
					child[meeple*pop.pixels + x] = pop.arr[parent2*pop.pixels + x];
			}
		}
		
			//////////////////////////////////////////////////////////////////////////
		else
		{
			int parent;
			if (dart() < 0.5)
				parent = parent1;
			else
				parent = parent2;

			for (int x = 0; x < pop.pixels; x++) 
				child[meeple*pop.pixels + x] = pop.arr[parent*pop.pixels + x];
		}
	}
	
	for (int b = 0; b < pop.length; b++)
		pop.arr[b] = child[b];

	delete[] child;
	delete[] Fitness;
}

/*

/////Spare Code for Fitness Clearing

///////////////////		Clearing (Fitness Sharing)		///////////////
std::vector<int> sorted(pop.population);
for (int i = 0; i < pop.population; i++)
sorted[i] = i;


int capacity = 8;
double dist = 1;
pop.elites[pop.best] = true;

sort(Fitness, sorted, pop.population);
for (int i = 0; i < pop.population; ++i)
if (Fitness[sorted[i]] > 0)
{
int winners = 1;
for (int j = i + 1; j < pop.population; ++j)
if (Fitness[sorted[j]] > 0)
{
double d = distance(pop, i, j);
if (d < dist)
{
if (winners < capacity)
winners++;
else
Fitness[sorted[j]] = 0.0;

pop.elites[sorted[j]] = false;
}
else if (j < pop.population / 2)
pop.elites[sorted[j]] = true;
}
}
*/