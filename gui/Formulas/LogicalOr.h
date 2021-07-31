#pragma once

#include <cfloat>
#include <stdexcept>

#include "Formula.h"

class LogicalOr : public Formula {
 public:
    LogicalOr();
    double compute(const std::vector<double>& dataInputs);
};