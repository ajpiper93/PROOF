#pragma once

#include <cmath>
#include "classes.h"
#include <vector>

double avg(double arr[], int size)
{
	double sum = 0;
	for (int i = 0; i < size; ++i)
		sum += arr[i];

	return sum / size;
}

double sigma(double arr[], int size)
{
	double sum = 0;
	double mean = avg(arr, size);
	for (int i = 0; i < size; ++i)
		sum += pow(arr[i] - mean, 2);

	return pow(sum / size, 0.5);
}

void sort(std::vector<double> &vec, std::vector<int> &sorted, int vec_size)
{
	for (int i = 0; i < vec_size; ++i)
	{
		int count = 0;
		for (int elem = 0; elem < vec_size - 1; ++elem)
		{
			if (vec[sorted[elem]] < vec[sorted[elem + 1]])
			{
				int temp = sorted[elem];
				sorted[elem] = sorted[elem + 1];
				sorted[elem + 1] = temp;
				++count;
			}
		}
		if (count == 0)
			break;
	}
}

double distance(species &vec, int vec1, int vec2)
{
	double dist = 0;
	for (int b = 0; b < vec.pixels; b++)
		dist += pow(vec.arr[vec1*vec.pixels + b] - vec.arr[vec2*vec.pixels + b], 2);

	return sqrt(dist);
}
