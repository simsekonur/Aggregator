#include<vector>
#include<string>
#include<iostream>
#include <algorithm>    // std::sort
float findMedian(const std::vector<float> &v, std::string userChoice);
float findSum(const std::vector<float>& v,float userThreshold, std::string lessOrBig);
float findSum(const std::vector<float> &v);
float findMin(const std::vector<float>& v,float userThreshold, std::string lessOrGreater);
float findMin(const std::vector<float> &v);
void swap(int *xp, int *yp);
void bubbleSort(int arr[], int n);