#include "BasicOperation.hpp"

BasicOperation::BasicOperation(Operation op) : op(op), Formula() {}

double BasicOperation::compute(const std::vector<double>& dataInputs) {
	double result = 0;
	switch (op) {
	case Add:
	case Or:
		for (double number : dataInputs)
			result += number;
		break;
	case Multiply:
	case And:
		for (double number : dataInputs)
			result *= number;
		break;
	}
	return result;
}
