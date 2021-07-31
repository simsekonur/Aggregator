#pragma once
#include "Formula.hpp"
#include <cfloat>

class Min : public Formula {
public:
	Min();
	double compute(const std::vector<double>& dataInputs);
};