#include "FormulaNode.hpp"
#include <iostream>

FormulaNode::FormulaNode() {
	resultValue = 0;
}

FormulaNode::FormulaNode( Formula* formula) : formula(formula), resultValue(0) {}

bool FormulaNode::isLeaf() {
    return false;
}
void FormulaNode::setFormula(Formula * formula) {
	this->formula = formula;
}
double FormulaNode::getResultValue() const {
	return resultValue;
}

std::vector<double> FormulaNode::evaluate() {
    std::vector<double> inputList;
    for (BaseNode* child: children) {
        std::vector<double> result = child->evaluate();
        inputList.insert(inputList.end(), result.begin(), result.end());
    }
    resultValue = formula->compute(inputList);
	return std::vector<double> {resultValue};
}
