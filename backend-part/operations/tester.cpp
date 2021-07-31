#include"operations.h"

int main(void){

    // the test cases for find min  

    // THE TEST CASE 1 
    std::vector<float> v;
    v.push_back(-1); // let the first guy to be missing data
    v.push_back(1.99);
    v.push_back(2.0);
    v.push_back(3.2);
   
    // THE TEST CASE 2
     std::vector<float> v2;
    v2.push_back(-1); // let the first guy to be missing data
    v2.push_back(1.90);
    v2.push_back(2.2);
    v2.push_back(3.2);
    v2.push_back(1.5);



    std::cout << findMin(v,2,"default") << std::endl; // shoulbe 1.99
    // find the min if the data is bigger than 1.99
    std::cout << findMin(v2,1.99,"big") << std::endl; //2.2
    // find the min if the data is less than 1.99
    std::cout << findMin(v2,1.99,"less") << std::endl; //1.5


}