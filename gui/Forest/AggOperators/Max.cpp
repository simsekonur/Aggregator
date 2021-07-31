#include "Max.h"
#include <cfloat>

double Max::compute(const std::vector<double>& dataInputs) {
    double max = DBL_MIN;
    int size = dataInputs.size();
    for (int i = 0; i < size; i++)
        if (dataInputs[i] > max)
            max = dataInputs[i];
    return max;
};