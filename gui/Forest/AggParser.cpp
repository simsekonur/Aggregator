#include <sstream>
#include <float.h>
#include <algorithm>
#include <functional>
#include <iostream>

#include "AggParser.hpp"
#include "AggExceptions.hpp"

#include "AggOperators/Max.h"
#include "AggOperators/Min.h"
#include "AggOperators/Median.h"
#include "AggOperators/WeightedSum.hpp"
#include "AggOperators/Average.h"
#include "AggOperators/LogicalOr.h"
#include "AggOperators/LogicalAnd.h"
#include "AggOperators/CountNonZero.h"
#include "AggOperators/Mode.hpp"
#include "AggOperators/Multiply.hpp"
#include "AggOperators/Sum.h"

#include "agglextab.hpp"

double AggParser::getResult() const {
    return result;
}

void AggParser::setResult(double r) {
    result = r;
}
void AggParser::filterArgs(std::vector<double>& args, const Condition& cond) {
    double val = cond.value;
    std::function<bool(double)> filter;
    
    switch (cond.comp) {
        case Comp::lt:
            filter = [&val] (double x) {return !(x < val);};
            break;
        case Comp::lte:
            filter = [&val] (double x) {return !(x <= val);};
            break;
        case Comp::gt:
            filter = [&val] (double x) {return !(x > val);};
            break;
        case Comp::gte:
            filter = [&val] (double x) {return !(x >= val);};
            break;
        case Comp::ne:
            filter = [&val] (double x) {return !(x != val);};
            break;
    }

    auto filteredEnd = std::remove_if(args.begin(), args.end(), filter);
    args.erase(filteredEnd, args.end());
}

void AggParser::fillNumberArgs(std::vector<double>& args, const std::vector<AggExp*>& expressions) {
    for (auto exp : expressions) {
        if (exp->isNumber()) {
            args.push_back(exp->getNumber());
        }
        else {
            bool variableFound = false;
            const std::string& varName = exp->getName();

            auto nIterator = numberVariables.find(varName);
            if (nIterator != numberVariables.end()) {
                args.push_back(numberVariables[varName]);
                variableFound = true;
            }

            auto vIterator = vectorVariables.find(varName); 
            if (vIterator != vectorVariables.end() && !variableFound) {
                const std::vector<double>& variable = *vectorVariables[varName];
                args.insert(args.begin(), variable.begin(), variable.end());
                variableFound = true;
            }
            
            if (!variableFound) {
                std::stringstream err;
                err << "Invalid argument: \"" << varName << "\"";
                throw parser_exception(err.str());
            }
        }
    }
}

double AggParser::getNumberVariable(const std::string& name) const {
    auto nIterator = numberVariables.find(name);
    if (nIterator != numberVariables.end()) {
        return numberVariables.at(name);
    }
    else {
        std::stringstream err;
        err << "\"" << name << "\" is not a singular value variable.";
        throw parser_exception(err.str());
    }
}

void AggParser::updateVariable(const std::string& name, double value) {
    auto it = numberVariables.find(name);
    if (it == numberVariables.end()) {
        std::stringstream err;
        err << "Variable \"" << name << "\" cannot be updated, because it does not exist.";
        throw parser_exception(err.str());
    }
    else {
        auto& pair = *it;
        pair.second = value;
    }
}

void AggParser::setVariable(const std::string& name, double value) {
    if (numberVariables.find(name) == numberVariables.end()) {
        numberVariables.emplace(name, value);
    }
    else {
        std::stringstream err;
        err << "Variable \"" << name << "\" already exists.";
        throw parser_exception(err.str());
    }
}

void AggParser::setVariable(const std::string& name, const std::vector<double>* values) {
    if (vectorVariables.find(name) == vectorVariables.end()) {
        vectorVariables.emplace(name, values);
    }
    else {
        std::stringstream err;
        err << "Variable \"" << name << "\" already exists.";
        throw parser_exception(err.str());
    }
}

double AggParser::apply(const char* functionName, const std::vector<AggExp*>& exps, const Condition& cond) {
    std::string function(functionName);
    std::vector<double> args;
    fillNumberArgs(args, exps);
    
    if (cond.comp != Comp::none) {
        filterArgs(args, cond);
    }

    if (function.compare("max") == 0) {
        return Max::compute(args);
    }
    else if (function.compare("min") == 0) {
        return Min::compute(args);
    }
    else if (function.compare("average") == 0) {
        return Average::compute(args);
    }
    else if (function.compare("median") == 0) {
        return Median::compute(args);
    }
    else if (function.compare("mode") == 0) {
        return CountNonZero::compute(args);
    }
    else if (function.compare("or") == 0) {
        return LogicalOr::compute(args);
    }
    else if (function.compare("and") == 0) {
        return LogicalAnd::compute(args);
    }
    else if (function.compare("sum") == 0) {
        return Sum::compute(args);
    }
    else if (function.compare("multiply") == 0) {
        return Multiply::compute(args);
    }
    else if (function.compare("non_zero") == 0) {
        return CountNonZero::compute(args);
    }
    else {
        std::stringstream err;
        err << "Unknown aggregation operator: " << function;
        throw parser_exception(err.str());
    }
}

double AggParser::apply(const char* functionName, const std::vector<AggExp*>& exps, const std::vector<AggExp*>& secExps) {
    std::string function(functionName);
    std::vector<double> args;
    std::vector<double> secArgs;
    fillNumberArgs(args, exps);
    fillNumberArgs(secArgs, secExps);
    //std::reverse(secArgs.begin(), secArgs.end());

    if (function.compare("weighted_sum") == 0) {
        return WeightedSum::compute(args, secArgs);
    }
    else {
        std::stringstream err;
        err << "Unknown aggregation operator: " << function;
        throw parser_exception(err.str());
    }
}

void AggParser::parse(const std::string& expString) {
    yyscan_t scanner;
    yylex_init(&scanner);
    YY_BUFFER_STATE buf = yy_scan_string(expString.c_str(), scanner);
    yyparse(scanner, *this);
    yylex_destroy(scanner);
}

void AggParser::printWeights() const {
    for (auto pair: numberVariables) {
        std::cout << "w_" << pair.first << "=" << pair.second << ", ";
    }
}
