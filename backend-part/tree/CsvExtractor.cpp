#include "CsvExtractor.hpp"
#include <iostream>
#include <fstream>

bool CsvExtractor::isCsvFile(std::string fileName) {
	int size = fileName.length();
	if (size > 4) {
		return fileName.substr(size - 3, 3).compare("csv") == 0;
	}
	return false;
}

CsvExtractor::CsvExtractor(std::string path) : filePath(path) {}

std::vector<double> CsvExtractor::getDataSet() {
	std::vector<double> dataSet;
	std::ifstream file;
	if (isCsvFile(filePath)) {
		file.open(filePath.c_str());
		std::string tmpStr;
		file >> tmpStr;
		std::string number;
		while (file >> number) {
			try {
				dataSet.push_back(std::stof(number));
			}
			catch (...) {
				throw std::invalid_argument("File contains non number lines");
			}
		}
		return dataSet;
	}
	else {
		throw std::invalid_argument("Input is not a csv file.");
	}
}
