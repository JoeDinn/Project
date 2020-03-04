#pragma once
#include "Optimiser.h"
#include <list>
class Greedy :
	public Optimiser
{
public:
	Greedy(Similarity &similarity_);
	Solution optimise(Solution fragments);
	~Greedy();
};

