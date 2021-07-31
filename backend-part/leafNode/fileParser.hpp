#include "LeafNode.hpp"
#include <iostream>
#include <fstream>

class fileParser
{
private:
    LeafNode l1;
    std::string fileName;

public:
   fileParser(std::string f);
   
    std::vector<float> loadFromTxt();
  
    
};