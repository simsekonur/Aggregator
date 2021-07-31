#pragma once

#include <cfloat>
#include <iostream>

#include "Formula.h"

class Max : public Formula {
 public:
    Max();
    double compute(const std::vector<double>& dataInputs);
};