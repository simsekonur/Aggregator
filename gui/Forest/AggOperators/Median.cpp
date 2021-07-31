#include "Median.h"

void Median::swap(double *xp, double *yp) {
    double temp = *xp;
    *xp = *yp;
    *yp = temp;
}

// A function to implement bubble sort
void Median::bubbleSort(std::vector<double> &arr, int n) {
    int i, j;
    for (i = 0; i < n - 1; i++)

        // Last i elements are already in place
        for (j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1])
                swap(&arr[j], &arr[j + 1]);
}

double Median::findMedian(const std::vector<double> &v, std::string userChoice) {
    int l = v.size();

    //copy your all data to some another vector
    std::vector<double> v2;

    for (int i = 0; i < l; i++) {
        if (v[i] != -1.0) {
            v2.push_back(v[i]);
        }
    }

    l = v2.size();
    bubbleSort(v2, l);

    if (l % 2 == 1) {
        return v2[(l - 1) / 2];
    }
    //Elements are just those we want right now
    else {
        {  // even number of elements
            if (userChoice == "left") {
                return v2[(l / 2) - 1];
            }
            else if (userChoice == "right") {
                return v2[l / 2];
            }
            else if (userChoice == "average") {
                return (v2[l / 2] + v2[(l / 2) - 1]) / 2.0;
            }
        }
    }
}
double Median::compute(const std::vector<double> &dataInputs) {
    return findMedian(dataInputs, "average");
}