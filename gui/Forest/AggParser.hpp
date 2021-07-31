#pragma once

#include <string>
#include <vector>
#include <string>
#include <cfloat>
#include <map>
#include "AggExp.hpp"

namespace Comp {
    enum Comparator {
        lt,
        gt,
        lte,
        gte,
        ne,
        none
    };
}

struct Condition {
    Comp::Comparator comp;
    double value;
    Condition(Comp::Comparator _comp, double _value) : comp(_comp), value(_value) {}
};

class AggParser {
private:
    std::map<std::string, double> numberVariables;
    std::map<std::string, const std::vector<double>* > vectorVariables;
    double result;
    
    void fillNumberArgs(std::vector<double>& args, const std::vector<AggExp*>& expressions);
    void filterArgs(std::vector<double>& args, const Condition& cond);

public:
    void setVariable(const std::string& name, double value);
    void setVariable(const std::string& name, const std::vector<double>* values);
    void updateVariable(const std::string& name, double value);

    double getNumberVariable(const std::string& name) const;

    double getResult() const;
    void setResult(double res);

    double apply(const char* function, const std::vector<AggExp*>& args, const Condition& cond);
    double apply(const char* function, const std::vector<AggExp*>& args, const std::vector<AggExp*>& secArgs);
    void parse(const std::string& expString);

    void printWeights() const;
};

