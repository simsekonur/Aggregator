#include "Tree.h"

#include <iostream>
#include <stdexcept>

#include "DataNode.h"

Tree::Tree() {
    root = NULL;
}

Tree::Tree(FormulaNode* root) {
    this->root = root;
    NodeList firstLevel;
    firstLevel.push_back(root);
    levelList.push_back(firstLevel);
}

Tree::~Tree() {
    delete root;
}

double Tree::evaluate() {
    return root->evaluate()[0];
}

void Tree::initialize() {
    return;
}

int Tree::getChildNodeLevel(BaseNode* nodesParent) {
    int index = 0;
    for (NodeList& list : levelList) {
        bool foundParent = false;
        for (BaseNode* parentNode : list) {
            if (parentNode == nodesParent) {
                foundParent = false;
                break;
            }
        }
        if (foundParent)
            break;
        index++;
    }
    return index+1;
}

void Tree::addNode(BaseNode* parent, BaseNode* child) {
    parent->addChild(child);
    int i = getChildNodeLevel(parent);
    if (i >= levelList.size()) {
        NodeList newList;
        newList.push_back(child);
        levelList.push_back(newList);
    }
    else {
        levelList[i].push_back(child);
    }
}

void Tree::removeSubTree(BaseNode* node, int level) {
    for (BaseNode* child : node->getChildren()) {
        removeSubTree(child, level + 1);
    }
    NodeList& list = levelList[level];
    for (int i = 0; i < list.size(); i++) {
        if (list[i] == node) {
            list.erase(list.begin() + i);
        }
    }
    delete node;
}

void Tree::clearLevelList() {
    for (int i = levelList.size() - 1; i >= 0; i++) {
        if (levelList[i].empty()) {
            levelList.erase(levelList.begin() + i);
        }
    }
}

void Tree::removeNode(BaseNode* parent, BaseNode* child) {
    parent->removeChild(child);
    int i = getChildNodeLevel(parent);
    removeSubTree(child, i);
    clearLevelList();
}

tinyxml2::XMLElement* Tree::createXMLElement(tinyxml2::XMLDocument& doc, BaseNode* node) {
    using namespace tinyxml2;
    XMLElement* element = doc.NewElement("Node");

    if (node->isLeaf()) {
        element->SetAttribute("type", "leaf");
    }
    else {
        element->SetAttribute("type", "formula");
        XMLElement* children = doc.NewElement("Children");
        element->InsertFirstChild(children);
        for (BaseNode* child : node->getChildren()) {
            XMLElement* childElement = createXMLElement(doc, child);
            children->InsertEndChild(childElement);
        }
    }
    return element;
}

void Tree::saveAsXML(const std::string& fileName) {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* tree = doc.NewElement("Tree");

    doc.InsertFirstChild(tree);
    tinyxml2::XMLElement* root = createXMLElement(doc, this->root);
    tree->InsertFirstChild(root);

    FILE* f = fopen(fileName.c_str(), "w");
    if (f)
        doc.SaveFile(f);
    else
        throw std::runtime_error("Error while saving XML file.");
}

BaseNode* Tree::builtNode(tinyxml2::XMLElement* element) {
    using namespace tinyxml2;
    if (element->Attribute("type", "data")) {
        DataNode* node = new DataNode();
        return node;
    }
    FormulaNode* node = new FormulaNode();
    for (XMLElement* child = element->FirstChildElement("Children")->FirstChildElement();
         child != NULL;
         child = child->NextSiblingElement()) {
        node->addChild(builtNode(child));
    }
    return node;
}

FormulaNode* Tree::builtFromXML(const std::string& fileName, QTreeWidget* widget) {
    using namespace tinyxml2;
    XMLDocument doc;
    XMLError status = doc.LoadFile(fileName.c_str());

    if (status == XML_SUCCESS) {
        XMLElement* root = doc.FirstChildElement()->FirstChildElement();
        FormulaNode* rootNode = new FormulaNode(widget);
        for (XMLElement* child = root->FirstChildElement("Children")->FirstChildElement();
             child != NULL;
             child = child->NextSiblingElement()) {
            //rootNode->addChildNode(builtNode(child));
            // hata alıyoruz buradan
        }
        return rootNode;
    }
    else {
        throw std::runtime_error("Error while loading XML file.");
    }
}