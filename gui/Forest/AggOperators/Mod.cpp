#include "Mode.hpp"

double Mode::compute(const std::vector<double>& args) {
    std::map<double, int> table;
    for (double arg: args) {
        auto it = table.find(arg);
        if (it == table.end()) {
            table.emplace(arg, 1);
        }
        else {
            auto& pair = *it;
            pair.second = pair.second + 1;
        }
    }

    int maxOccur = 0;
    double value = -1;
    for (auto& pair: table) {
        if (pair.second > maxOccur) {
            maxOccur = pair.second;
            value = pair.first;
        }
    }
    
    return value;
}