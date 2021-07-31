#include<vector>
#include<iostream>
class LeafNode {
    private :
        float normalInteger;//in case user want to enter only one int
                        //can be used for debugging purposes
        std::vector<float> allNumbers;

    public:
        LeafNode();
        LeafNode(std::vector<float> l2);
        float handleInsertion(float item);
        float handleRemove(float item);
        void displayDataInLeafNode(std::vector<float> l);
        void displayDataInLeafNode();
        std::vector<float> getAllData();

};

