#pragma once

#include <string>
#include <vector>

class Sum {
    static double findSum(const std::vector<double>& v);
    static double findSum(const std::vector<double>& v, double userThreshold, std::string lessOrBig);
 public:
    static double compute(const std::vector<double>& v);
};