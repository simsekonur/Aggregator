#pragma once

#include <string>
#include <vector>

#include "Formula.h"

class Median : public Formula {
 public:
    Median();
    void swap(double* a, double* b);
    void bubbleSort(std::vector<double>& ll, int n);
    double findMedian(const std::vector<double>& input, std::string str);
    double compute(const std::vector<double>& dataInputs);
};