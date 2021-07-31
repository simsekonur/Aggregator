#include <vector>
#include <iostream>
#include "BaseNode.hpp"
#include "DataExtractor.hpp"

class DummyLeaf : public BaseNode {
	std::vector<double> values;
	DataExtractor* extractor;
public:
	DummyLeaf(): BaseNode() {};
	DummyLeaf(const std::vector<double>& list) {
		values.insert(values.end(), list.begin(), list.end());
	}
	DummyLeaf(DataExtractor* dataExtractor) {
		extractor = dataExtractor;
		values = extractor->getDataSet();
	}
	std::vector<double> evaluate() {
		return values;
	}
	bool isLeaf() {
		return true;
	}
	void printValues() {
		for (double f : values) {
			std::cout << f << " ";
		}
		std::cout << std::endl;
	}
};