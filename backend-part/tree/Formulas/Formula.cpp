#include "Formula.hpp"

Formula::Formula() {}

Formula::Formula(const std::string& name) : name(name) {}

Formula::Formula(const std::vector<double>& formulaInputs) : formulaInputs(formulaInputs) {}

Formula::Formula(const std::string& name, const std::vector<double>& formulaInputs)
	: name(name), formulaInputs(formulaInputs) {}

double Formula::compute(const std::vector<double>& dataInputs) {
	return 0;
}
