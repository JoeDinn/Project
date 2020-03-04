#pragma once
#include "utilities.h"
class Metric
{
public:
	Metric();
	virtual double evaluate(Solution &solution, GroundTruth &ground_truth)= 0;
	~Metric();
};

