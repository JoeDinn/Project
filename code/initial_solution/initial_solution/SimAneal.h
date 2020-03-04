#pragma once
#include "utilities.h"
#include "Optimiser.h"
class SimAneal :
	public Optimiser
{
	double initial_temperature;
	int temperature_length;
	double cooling_factor;
	int num_non_improve;
public:
	SimAneal(Similarity &similarity_, double initial_temperature_ = 5861504, int temperature_length_ = 10, double cooling_factor_ = 0.1, int num_non_improve_ = 100);
	~SimAneal();
	Solution optimise(Solution fragments);
	double get_updated_cost(Solution &current_solution, double current_score, int index);
	void get_random_neighbour(Solution current_solution);
	double get_initial_cost(Solution initial_solution);
};

