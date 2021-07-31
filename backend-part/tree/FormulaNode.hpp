#ifndef FORMULA_NODE_H
#define FORMULA_NODE_H

#include "BaseNode.hpp"
#include "Formulas/Formula.hpp"

class FormulaNode : public BaseNode {
	Formula* formula;
	double resultValue;
public:
	FormulaNode();
	FormulaNode(Formula* formula);
	void setFormula(Formula* formula);
	bool isLeaf();
	double getResultValue() const;
	std::vector<double> evaluate();
};

#endif