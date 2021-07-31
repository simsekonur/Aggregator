#pragma once

#include <string>
#include <vector>

class Formula {
 protected:
    std::string name;
    std::vector<double> formulaInputs;

 public:
    Formula();
    Formula(const std::string& name);
    Formula(const std::vector<double>& formulaInputs);
    Formula(const std::string& name, const std::vector<double>& formulaInputs);
    virtual double compute(const std::vector<double>& dataInputs);
};