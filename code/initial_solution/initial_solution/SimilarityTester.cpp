#include "stdafx.h"
#include "SimilarityTester.h"
#include <vector>
typedef std::pair<std::string, long double> Fragment_cost;
void SimilarityTester::test(Solution &correct_solution, Similarity & similarity)
{
	for (int index = 0; index < correct_solution.size() - 1; index++)
	{
		std::vector<Fragment_cost> costs{};
		//iterate over each item
		for (int next = 0; next < correct_solution.size();  next++)
		{
			if (next != index)
			{//calculate cost
				costs.emplace_back(Fragment_cost(correct_solution[next].name,similarity.compare(correct_solution[index], correct_solution[next])));
			}
		}
		//sort costs
		std::sort(costs.begin(), costs.end(), [](auto &left, auto &right) {
			return left.second < right.second;
		});
		//output position of correct next item
		for (int i=0; i< costs.size(); ++i)
		{
			if (costs[i].first == correct_solution[index + 1].name)
			{
				std::cout << i << std::endl;
			}
		}





	}
}

SimilarityTester::SimilarityTester()
{
}


SimilarityTester::~SimilarityTester()
{
}
