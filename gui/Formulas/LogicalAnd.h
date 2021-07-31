#pragma once

#include <cfloat>
#include <stdexcept>

#include "Formula.h"

class LogicalAnd : public Formula {
   public:
    LogicalAnd();
    double compute(const std::vector<double>& dataInputs);
};