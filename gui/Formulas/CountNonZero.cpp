#include "CountNonZero.h"

CountNonZero::CountNonZero() : Formula() {}

double CountNonZero::compute(const std::vector<double>& dataInputs) {
    int counter = 0;
    int size = dataInputs.size();
    for (int i = 0; i < size; i++)
        if (dataInputs[i] != 0.0 && dataInputs[i] != -1.0)
            counter += 1;

    return counter;
};