#include "Tree.hpp"
#include <iostream>

Tree::Tree() {
	root = NULL;
}

Tree::Tree(FormulaNode * root) : root(root) {
}

double Tree::evaluate()
{
	return root->evaluate()[0];
}

void Tree::printSnapshot() {
	printTreeResult(root, 0);
}

void Tree::printTreeResult(FormulaNode* node, int level) {
	std::cout << level << " -> " << node->getResultValue() << std::endl;
	for (BaseNode* child : node->getChildren()) {
		if (!child->isLeaf()) {
			printTreeResult((FormulaNode*) child, level + 1);
		}
	}
}
