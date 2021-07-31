#include "Multiply.hpp"

double Multiply::compute(const std::vector<double>& args) {
    double result = 1.0;
    for (auto arg: args)
        result *= arg;
    return result;
}