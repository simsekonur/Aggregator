#include "LogicalOr.h"

LogicalOr::LogicalOr() : Formula() {}

double LogicalOr::compute(const std::vector<double>& dataInputs) {
    int i = 0;
    bool flag = true;
    //whenever you see a 1, you are done with the loop
    // go outside
    int size = dataInputs.size();
    while (i < size && flag) {
        if (!(dataInputs[i] >= 0 && dataInputs[i] <= 1.0)) {
            throw std::invalid_argument("All the data should be in the range 1 and 0\n");
        }
        else if (dataInputs[i] == 1.0) {  // I now found a 1.0
            flag = false;
        }

        i++;
    }
    if (flag == false) {
        return 1;
    }
    else {
        return 0;
    }
};