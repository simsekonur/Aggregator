#include "Sum.h"

double Sum::findSum(const std::vector<double> &v) {
    double result = 0.0;
    int l = v.size();
    for (int i = 0; i < l; i++) {
        result += v[i];
    }
    return result;
}

double Sum::findSum(const std::vector<double> &v, double userThreshold, std::string lessOrBig) {
    std::vector<double> v2;
    int l = v.size();  // size is at l
    double sum = 0.0;
    if (lessOrBig == "default") {
        for (int i = 0; i < l; i++) {
            if (v[i] != -1.0) {
                v2.push_back(v[i]);  // only put the data that is not missing
            }
        }
    }
    else if (lessOrBig == "less") {
        for (int i = 0; i < l; i++) {
            if (v[i] != -1.0 && v[i] < userThreshold) {
                v2.push_back(v[i]);  // only put the data that satisfies the condition
            }
        }
    }
    else if (lessOrBig == "big") {
        for (int i = 0; i < l; i++) {
            if (v[i] != -1.0 && v[i] > userThreshold) {
                v2.push_back(v[i]);  // only put the data that satisfies the condition
            }
        }
    }
    sum = findSum(v2);
    return sum;
}

double Sum::compute(const std::vector<double> &v) {
    return findSum(v, 1, "default");
}