#include "stdafx.h"
#include "SimAneal.h"



SimAneal::SimAneal(Similarity &similarity_, double initial_temperature_, int temperature_length_, double cooling_factor_, int num_non_improve_) :
	Optimiser(similarity_),
	initial_temperature(initial_temperature_), temperature_length(temperature_length_), cooling_factor(cooling_factor_), num_non_improve(num_non_improve_)
{
}


SimAneal::~SimAneal()
{
}



Solution SimAneal::optimise(Solution fragments)
{
	Solution current_solution{ fragments };
	double current_cost{ get_initial_cost(current_solution) };
	std::cout << "At start " << current_cost << std::endl;
	double temperature{ initial_temperature };
	
	Solution best_solution{ current_solution };
	long double best_cost{ current_cost };

	int uphill_moves{ 0 };
	int count_non_improve{ 0 };
	while ((count_non_improve < num_non_improve) and (temperature > 0))
	{
		for (int i{}; i < temperature_length; ++i)
		{
			int index = random::rand_int(current_solution.size() - 2);
			long double neighbour_cost{ get_updated_cost(current_solution, current_cost, index) };
			long double cost_change{ neighbour_cost - current_cost };
			if (cost_change <= 0)
			{
				std::swap(current_solution[index], current_solution[index + 1]);
				current_cost = neighbour_cost;
			}
			else
			{
				double q{ random::rand_double(1) };

				if (q < exp( -cost_change / temperature))
				{
					std::swap(current_solution[index], current_solution[index + 1]);
					current_cost = neighbour_cost;
					uphill_moves += 1;
				}
			}
			if (current_cost < best_cost)
			{
				best_solution = current_solution;
				best_cost = current_cost;
				count_non_improve = 0;
			}
			else count_non_improve += 1;
		}
		temperature -= (cooling_factor * initial_temperature);
	}
	std::cout << "At end " << best_cost << std::endl;
	return best_solution;
}

double SimAneal::get_updated_cost(Solution &current_solution, double current_cost, int index)
{

	double old_cost{};
	double new_cost{};

	if (index > 0) old_cost += similarity.compare(current_solution[index - 1], current_solution[index]);
	if (index <	(current_solution.size() - 1)) old_cost += similarity.compare(current_solution[index], current_solution[index + 1]);
	if (index <	(current_solution.size() - 2)) old_cost += similarity.compare(current_solution[index + 1], current_solution[index + 2]);
	
	if (index > 0 and index < (current_solution.size() - 1)) new_cost += similarity.compare(current_solution[index - 1], current_solution[index + 1]);
	if (index <	(current_solution.size() - 1)) new_cost += similarity.compare(current_solution[index + 1], current_solution[index]);
	if (index <	(current_solution.size() - 2)) new_cost += similarity.compare(current_solution[index], current_solution[index + 2]);

	return current_cost + (new_cost - old_cost);
}

void SimAneal::get_random_neighbour(Solution current_solution)
{
	int index = random::rand_int(current_solution.size() - 1);
	std::swap(current_solution[index], current_solution[index + 1]);

}

double SimAneal::get_initial_cost(Solution solution)
{
	double total{ 0 };
	for (int i{ 0 }; i < solution.size() - 1; ++i)
	{
		total += similarity.compare(solution[i], solution[i+1]);
	}
	return total;
}