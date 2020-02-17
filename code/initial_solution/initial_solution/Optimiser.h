#pragma once
#include "utilities.h"
#include "Similarity.h"
class Optimiser
{
protected:
	Similarity &similarity;
public:
	Optimiser(Similarity &similarity_) : similarity(similarity_) {};
	virtual Solution optimise(Solution fragments) = 0;
};