#pragma once

#include <string>
#include <vector>


class Median {
 public:
    static void swap(double* a, double* b);
    static void bubbleSort(std::vector<double>& ll, int n);
    static double findMedian(const std::vector<double>& input, std::string str);
    static double compute(const std::vector<double>& dataInputs);
};