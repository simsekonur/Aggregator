#pragma once

#include <vector>

#include "./tinyxml/tinyxml2.h"
#include "BaseNode.h"
#include "FormulaNode.h"

typedef std::vector<BaseNode*> NodeList;
class Tree {
    FormulaNode* root;
    std::vector<NodeList> levelList;

    tinyxml2::XMLElement* createXMLElement(tinyxml2::XMLDocument& doc, BaseNode* node);
    BaseNode* Tree::builtNode(tinyxml2::XMLElement* element);
    int getChildNodeLevel(BaseNode* parentNode);
    void removeSubTree(BaseNode* node, int level);
    void clearLevelList();

 public:
    Tree();
    Tree(FormulaNode* root);
    ~Tree();

    double evaluate();
    void addNode(BaseNode* parent, BaseNode* child);
    void removeNode(BaseNode* parent, BaseNode* child);

    void saveAsXML(const std::string& fileName);
    FormulaNode* builtFromXML(const std::string& fileName, QTreeWidget* widget);

    void initialize();
};