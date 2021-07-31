#include"operations.h"

int main(void){

    std::vector<float> v;
    v.push_back(1.99); 
    v.push_back(2.0);
    v.push_back(5.2);
    v.push_back(5.3);

    v.push_back(-1.0); // should be ignored
    v.push_back(3.2);

   
    //1.99 2.0  3.2 5.2 5.3 
    std::cout << findMedian(v,"left") << std::endl; // we have odd number of elements
                                                    // so we don't even need left or right
                                                    // THE TEST CASE 2
     std::vector<float> v2;
    v2.push_back(-1); // let the first guy to be missing data
    v2.push_back(1.5);
    v2.push_back(1.0);
    v2.push_back(2.0);
    v2.push_back(3.2);
    // 1.0 1.5 2.0 3.2
    std::cout << findMedian(v2,"average") << std::endl;
    std::cout << findMedian(v2,"left") << std::endl;
    std::cout << findMedian(v2,"right") << std::endl;


    // Everything good with median 
    
}