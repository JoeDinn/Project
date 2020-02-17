#include "stdafx.h"
#include "greedy.h"


greedy::greedy(Similarity &similarity_):
	Optimiser(similarity_)
{
}

Solution greedy::optimise(Solution fragments)
{
	//list of indexs
	std::list<int> indexes{};
	for (int i{}; i < fragments.size(); ++i) indexes.push_back(i);
	Solution optimal_solution(fragments.size());
	//find leftmost
	int current{ leftMost(fragments) };
	indexes.remove(current);
	std::cout << "Start " << fragments[current].name << std::endl;
	optimal_solution[0] = fragments[current];
	//Find best neighbour from remaining fragments

	for(int i{1}; i < fragments.size(); ++i)
	{
		int best_index{ indexes.front() };
		long double best_score{ 1000000000 };
		for (int index : indexes)
		{
			long double current_score{ similarity.compare(fragments[current], fragments[index]) };
			if (current_score < best_score)
			{
				best_index = index;
				best_score = current_score;
			}
		}
		std::cout << fragments[current].name << " " << fragments[best_index].name << std::endl;
		optimal_solution[i] = fragments[best_index];
		current = best_index;
		indexes.remove(current);

	}
	//find optimal adjacent strip
	//
	return optimal_solution;
}


greedy::~greedy()
{
}
