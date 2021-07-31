#include"operations.h"

int main(void){

    // THE TEST CASE 1
    std::vector<float> v;
    v.push_back(1.0); 
    v.push_back(2.0);
    v.push_back(-1.0); // missing 
    v.push_back(3.0);
    
    
    std::cout << findSum(v,2,"default") << std::endl; // should be 6 

    // THE TEST CASE 2
     std::vector<float> v2;
    v2.push_back(-1); // let the first guy to be missing data
     v2.push_back(1.5);
    v2.push_back(1.90);
    v2.push_back(2.2);
    v2.push_back(3.2);
   

    std::cout << findSum(v2,2.0,"less")<< std::endl; //3.40
    std::cout << findSum(v2,2.0,"big")<< std::endl; // 5.4

    //Conclude that everything is OK, with find sum.



     
}