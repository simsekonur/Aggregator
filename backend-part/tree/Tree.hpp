#include "BaseNode.hpp"
#include "FormulaNode.hpp"

class Tree {
	FormulaNode *root;
	void printTreeResult(FormulaNode* node, int level);
public:
	Tree();
	Tree(FormulaNode *root);
	double evaluate();
	void printSnapshot();
};