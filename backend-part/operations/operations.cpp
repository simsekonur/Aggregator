#include "operations.h"
#include "missingData.h"
void swap(float *xp, float *yp)  
{  
    float temp = *xp;  
    *xp = *yp;  
    *yp = temp;  
}  
  
// A function to implement bubble sort  
void bubbleSort(std::vector<float> &arr, int n)  
{  
    int i, j;  
    for (i = 0; i < n-1; i++)      
      
    // Last i elements are already in place  
    for (j = 0; j < n-i-1; j++)  
        if (arr[j] > arr[j+1])  
            swap(&arr[j], &arr[j+1]);  
}  
  

float findMedian(const std::vector<float> &v, std::string userChoice)
{
    
    int l = v.size();

    
    //copy your all data to some another vector
    std::vector<float> v2 ;
    handleMissing(v,v2);
    
    l = v2.size();
    bubbleSort(v2,l);

    if (l % 2 == 1)
    {
        return v2[(l - 1) / 2];
    }
    //Elements are just those we want right now
    else
    {
        { // even number of elements
            if (userChoice == "left")
            {
                return v2[(l / 2) - 1];
            }
            else if (userChoice == "right")
            {
                return v2[l / 2];
            }
            else if (userChoice == "average")
            {
                return (v2[l / 2] + v2[(l / 2) - 1]) / 2.0;
            }
        }
    }
}
float findSum(const std::vector<float> &v)
{
    float result = 0.0;
    int l = v.size();
    for (int i = 0; i < l; i++)
    {
        result+=v[i];
    }
    return result;
}
float findSum(const std::vector<float> &v, float userThreshold, std::string lessOrBig)
{
    std::vector<float> v2;
    int l = v.size(); // size is at l
    float sum =0.0;
    if (lessOrBig == "default")
    {
        handleMissing(v,v2);
    }
    else if (lessOrBig == "less")
    {
        for (int i = 0; i < l; i++)
        {
            if (v[i] != -1.0 && v[i] < userThreshold)
            {
                v2.push_back(v[i]); // only put the data that satisfies the condition
            }
        }
    }
    else if (lessOrBig == "big")
    {
        for (int i = 0; i < l; i++)
        {
            if (v[i] != -1.0 && v[i] > userThreshold)
            {
                v2.push_back(v[i]); // only put the data that satisfies the condition
            }
        }
    }
    else
    {
        std::cout << "Reconsider the user condition.\n";
    }
    sum = findSum(v2);
    return sum;
}

float findMin(const std::vector<float> &v)
{
    float min = v[0];
    int l = v.size();
    for (int i = 0; i < l; i++)
    {
        if (v[i] < min)
        {
            min = v[i];
        }
    }
    return min;
}
float findMin(const std::vector<float> &v, float userThreshold, std::string lessOrBig)
{

    std::vector<float> v2;
    int l = v.size(); // pre calculate it for optimization
    float min = 0.0;
    if (lessOrBig == "default")
    {
        handleMissing(v,v2);
        min = findMin(v2);
    }
    else if (lessOrBig == "big")
    {
        for (int i = 0; i < l; i++)
        {
            if (v[i] != -1.0 && v[i] > userThreshold)
            {
                v2.push_back(v[i]); // do not use v anymore
            }
        }
        min = findMin(v2);
    }
    else if (lessOrBig == "less")
    {
        for (int i = 0; i < l; i++)
        {
            if (v[i] != -1.0 && v[i] < userThreshold)
            {
                v2.push_back(v[i]); // do not use v anymore
            }
        }
        min = findMin(v2);
    }

    else
    {
        std::cout << "Something wrong about the user conditon.\n";
    }

    return min;
}
