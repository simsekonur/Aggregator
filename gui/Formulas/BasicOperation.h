#pragma once
#include <vector>

#include "Formula.h"
#include "Operation.h"

class BasicOperation : public Formula {
 private:
    Operation op;

 public:
    BasicOperation(Operation op);

    double compute(const std::vector<double>& dataInputs);
};