#pragma once
#include "Metric.h"
class neighbour_comparison :
	public Metric
{
public:
	double evaluate(Solution &solution, GroundTruth &ground_truth);
	bool correct_allignment(Fragment &left_fragment, Fragment&right_fragment, GroundTruth & ground_truth);
	neighbour_comparison();
	~neighbour_comparison();
};

