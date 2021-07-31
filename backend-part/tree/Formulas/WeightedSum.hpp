#pragma once
#include "Formula.hpp"

class WeightedSum : public Formula {
private:
	bool isFormulaInputsValid(const std::vector<double>& dataInputs);
public:
	WeightedSum(const std::vector<double>& formulaInputs);

	double compute(const std::vector<double>& dataInputs);
};