#pragma once

#include <string>
#include <vector>

#include "Formula.h"

class Sum : public Formula {
 public:
    Sum();
    double findSum(const std::vector<double>& v);
    double findSum(const std::vector<double>& v, double userThreshold, std::string lessOrBig);
    double compute(const std::vector<double>& v);
};