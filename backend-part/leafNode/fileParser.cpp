#include"fileParser.hpp"

fileParser::fileParser(std::string f)
{
    this->fileName = f;
}

std::vector<float> fileParser::loadFromTxt()
{
    std::ifstream file(this->fileName);
    float data;
    std::string s;

    while (std::getline(file, s))
    {
        //Convert this string into float
        //Add it to the vector
        data = std::stof(s);
        l1.handleInsertion(data);
    }
    return this->l1.getAllData();
}