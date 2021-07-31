#pragma once

#include <cfloat>

#include "Formula.h"

class CountNonZero : public Formula {
 public:
    CountNonZero();
    double compute(const std::vector<double>& dataInputs);
};