#pragma once

#include <cfloat>

#include "Formula.h"

class Min : public Formula {
 public:
    Min();
    double compute(const std::vector<double>& dataInputs);
};