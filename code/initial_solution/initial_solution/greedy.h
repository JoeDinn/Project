#pragma once
#include "Optimiser.h"
#include <list>
class greedy :
	public Optimiser
{
public:
	greedy(Similarity &similarity_);
	Solution optimise(Solution fragments);
	~greedy();
};

