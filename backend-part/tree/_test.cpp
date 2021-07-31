#include "Tree.hpp"
#include "FormulaNode.hpp"
#include "_DummyLeaf.hpp"
#include "Formulas/Average.hpp"
#include "Formulas/Max.hpp"
#include "Formulas/Min.hpp"
#include "DataExtractor.hpp"
#include "CsvExtractor.hpp"
#include "Formulas/Formula.hpp"
#include <vector>
#include <iostream>

#define INPUT_FILE "input.csv"

int main() {
	Formula* avgFormula = new Average();
	Formula* maxFormula = new Max();
	Formula* minFormula = new Min();

	FormulaNode* root = new FormulaNode(avgFormula);
	FormulaNode* minFormulaNode = new FormulaNode(minFormula);
	FormulaNode* maxFormulaNode = new FormulaNode(maxFormula);

	// leaves with normal vectors
	DummyLeaf* leaf1 = new DummyLeaf(std::vector<double> {10, 20, 30});
	DummyLeaf* leaf2 = new DummyLeaf(std::vector<double> {40, 50, 60});
	DummyLeaf* leaf3 = new DummyLeaf(std::vector<double> {60, 70, 80});

	// leaf with data extracted from csv file
	DummyLeaf* csvLeaf = new DummyLeaf(new CsvExtractor(INPUT_FILE)); 

	root->addChild(minFormulaNode);
	root->addChild(maxFormulaNode);

	minFormulaNode->addChild(leaf1);
	minFormulaNode->addChild(csvLeaf);

	maxFormulaNode->addChild(leaf2);
	maxFormulaNode->addChild(leaf3);

	Tree tree(root);
	double result = tree.evaluate();
	std::cout << "Result: " << result << std::endl;
	tree.printSnapshot();

}