#pragma once
#include "Formula.hpp"

class Average : public Formula {
public:
	Average();
	double compute(const std::vector<double>& dataInputs);
};