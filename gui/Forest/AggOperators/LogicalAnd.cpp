#include "LogicalAnd.h"

double LogicalAnd::compute(const std::vector<double>& dataInputs) {
    int i = 0;
    bool flag = true;
    //whenever you see a 0, you are done with the loop
    // go outside
    int size = dataInputs.size();
    while (i < size && flag) {
        if (!(dataInputs[i] >= 0 && dataInputs[i] <= 1.0)) {
            throw std::invalid_argument("All the data should be in the range 1 and 0\n");
        }
        else if (dataInputs[i] == 0.0) {  // I now found a 0.0
            flag = false;
        }

        i++;
    }
    if (flag == false) {
        return 0;
    }
    else {
        return 1;
    }
};