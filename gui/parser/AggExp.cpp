#include "AggExp.hpp"

AggExp::AggExp(const char* name) {
    this->name = name;
    _isNumber = false;
}

AggExp::AggExp(double number) {
    this->number = number;
    _isNumber = true;
}

double AggExp::getNumber() const {
    return number;
}

const std::string& AggExp::getName() const {
    return name;
}

bool AggExp::isNumber() const {
    return _isNumber;
}

