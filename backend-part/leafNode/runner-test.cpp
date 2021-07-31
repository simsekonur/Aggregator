#include<iostream>
#include"LeafNode.hpp"
using namespace std;

int main(void){

    std::vector<float> v ;
    v.push_back(1.0);
    v.push_back(2.0);
    v.push_back(3.0);
    LeafNode l1(v); // this is the leaf node 

    LeafNode l2; // -1.0 is assigned to all elements

    l2.displayDataInLeafNode();


    l2.handleInsertion(1.0);
    l2.handleInsertion(5.2);
    l2.handleInsertion(3.0);

    l1.displayDataInLeafNode();
    l2.displayDataInLeafNode();

    int answer1= l2.handleRemove(99.0); // should return -1
    int answer2= l2.handleRemove(5.0); // should return 5
    cout << "answer1: " << answer1 << " , answer2: " << answer2 << endl;
    l2.displayDataInLeafNode();

    //New test cases related to .txt reading 

    fileParser f("input.txt");
    std::vector<float> fromTxt= f.loadFromTxt();
    LeafNode l3(fromTxt);
    l3.displayDataInLeafNode();



}