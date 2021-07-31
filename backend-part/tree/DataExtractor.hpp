#ifndef DATA_EXTRACTOR
#define DATA_EXTRACTOR

#include <vector>

class DataExtractor {
public:
	virtual std::vector<double> getDataSet() = 0;
};

#endif