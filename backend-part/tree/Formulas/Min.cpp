#include "Min.hpp"

Min::Min() : Formula() {}

double Min::compute(const std::vector<double>& dataInputs) {
	double min = DBL_MAX;
	int size = dataInputs.size();
	for (int i = 0; i < size; i++)
		if (dataInputs[i] < min)
			min = dataInputs[i];
	return min;
};