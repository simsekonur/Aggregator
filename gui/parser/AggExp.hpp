#pragma once

#include <string>
#include <vector>

class AggExp {
private:    
    bool _isNumber;
    double number;
    std::string name;

public:
    bool isNumber() const;
    const std::string& getName() const;
    double getNumber() const;
    AggExp(const char* name);
    AggExp(double number);
};