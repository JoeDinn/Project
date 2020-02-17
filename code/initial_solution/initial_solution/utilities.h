#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <random> // for std::mt19937
#include <ctime> // for std::time
#include <iostream>
#include "Fragment.h"

typedef std::vector<Fragment> Solution;

namespace random
{
	// Initialize our mersenne twister with a random seed based on the clock (once at system startup)
	static std::mt19937 mersenne(static_cast<std::mt19937::result_type>(std::time(nullptr)));

	static int rand_int(int max)
	{
		std::uniform_int_distribution<> rand_gen(0, max); // we can create a distribution in any function that needs it
		return rand_gen(random::mersenne); // and then generate a random number from our global generator
	}
	static int rand_int(int min, int max)
	{
		std::uniform_int_distribution<> rand_gen(min, max); // we can create a distribution in any function that needs it
		return rand_gen(random::mersenne); // and then generate a random number from our global generator
	}

	static double rand_double(int max)
	{
		std::uniform_real_distribution<double> rand_gen(0, max); // we can create a distribution in any function that needs it
		return rand_gen(random::mersenne); // and then generate a random number from our global generator
	}
	static double rand_double(int min, int max)
	{
		std::uniform_real_distribution<double> rand_gen(min, max); // we can create a distribution in any function that needs it
		return rand_gen(random::mersenne); // and then generate a random number from our global generator
	}
}


void output(Solution &solution);
void log(Solution &solution);

void shuffle(Solution &solution);

int leftMost(Solution &solution);