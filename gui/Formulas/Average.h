#pragma once

#include "Formula.h"

class Average : public Formula {
 public:
    Average();
    double compute(const std::vector<double>& dataInputs);
};