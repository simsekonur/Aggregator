#include "WeightedSum.h"

#include <stdexcept>

WeightedSum::WeightedSum(const std::vector<double>& formulaInputs) : Formula(formulaInputs) {}

bool WeightedSum::isFormulaInputsValid(const std::vector<double>& dataInputs) {
    double sum = 0;
    if (formulaInputs.size() != dataInputs.size())
        throw std::invalid_argument("Data and weights array must have same size.");

    for (double weight : this->formulaInputs) {
        if (weight < 0 || weight > 1)
            return false;
        sum += weight;
    }
    if (sum == 1.0)
        return true;
    else
        return false;
}

double WeightedSum::compute(const std::vector<double>& dataInputs) {
    if (isFormulaInputsValid(dataInputs)) {
        double sum = 0;
        int size = dataInputs.size();
        for (int i = 0; i < size; i++) {
            sum += dataInputs[i] * formulaInputs[i];
        }
        return sum;
    }
    else {
        throw std::invalid_argument("Sum of weights cannot be greater than 1.");
    }
}