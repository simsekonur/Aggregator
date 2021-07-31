#include"LeafNode.hpp"


LeafNode::LeafNode(){
    for (int i=0; i< this->allNumbers.size();i++){
        this->allNumbers.push_back(-1.0);
    }
    this->normalInteger=-1.0;

    
}
LeafNode::LeafNode(std::vector<float> l2){

    for (int i=0; i< l2.size();i++){
        this->allNumbers.push_back(l2[i]);
    }
    this->normalInteger=-1.0;

}
//I 'm going to return item that is inserted 
float LeafNode::handleInsertion(float item){
    this->normalInteger = item ;
    this->allNumbers.push_back(item);
    return item; 

}
//I 'm going to return item that is removed
float LeafNode::handleRemove(float item){
    bool flag= false ;
    for(auto it = this->allNumbers.begin(); it != this->allNumbers.end();it++){
        if(*it == item){
            this->allNumbers.erase(it);
            flag=true;
        }
    }
    if(flag ==false){ // we are not able to remove since item not exists
        return -1.0;
    }
    this->normalInteger=-1.0;
    return item;
}
void LeafNode::displayDataInLeafNode(){
    std::cout<< "****************************\n";
    for(auto it = this->allNumbers.begin(); it != this->allNumbers.end();it++){
        std::cout << *it << std::endl;
    }
    std::cout << "***************************\n";
}
void LeafNode::displayDataInLeafNode(std::vector<float> l2){
    std::cout<< "****************************\n";
    for(auto it = l2.begin(); it != l2.end();it++){
        std::cout << *it << std::endl;
    }
    std::cout << "***************************\n";
}
std::vector<float> LeafNode::getAllData(){
    return this->allNumbers;
}
