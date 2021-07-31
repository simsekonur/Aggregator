#ifndef CSV_EXTRACTOR
#define CSV_EXTRACTOR

#include "DataExtractor.hpp"
#include <string>
#include <vector>

class CsvExtractor : public DataExtractor {
private:
	std::string filePath;
	bool isCsvFile(std::string fileName);
public:
	CsvExtractor(std::string path);
	std::vector<double> getDataSet();
};

#endif