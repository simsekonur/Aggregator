#include "WeightedSum.hpp"
#include "../AggExceptions.hpp"

double WeightedSum::compute(const std::vector<double>& args, const std::vector<double>& secArgs) {
    if (args.size() != secArgs.size()) {
        throw parser_exception("Error while evaluating weighted_sum: lengths of argument lists don't match.");
    }
    double sum = 0;
    int size = args.size();
    for (int i = 0; i < size; i++)
        sum += args[i] * secArgs[i];
    return sum;
}
