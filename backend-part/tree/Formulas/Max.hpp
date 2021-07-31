#pragma once
#include "Formula.hpp"
#include <cfloat>
#include <iostream>

class Max : public Formula {
public:
	Max();
	double compute(const std::vector<double>& dataInputs);
};