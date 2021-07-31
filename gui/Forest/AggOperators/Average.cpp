#include "Average.h"

double Average::compute(const std::vector<double>& dataInputs) {
    double sum = 0;
    int size = dataInputs.size();
    for (int i = 0; i < size; i++)
        sum += dataInputs[i];
    return sum / size;
};