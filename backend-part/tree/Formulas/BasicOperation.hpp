#pragma once
#include <vector>
#include "Formula.hpp"
#include "Operation.hpp"

class BasicOperation : public Formula {
private:
	Operation op;
public:
	BasicOperation(Operation op);

	double compute(const std::vector<double>& dataInputs);
};
