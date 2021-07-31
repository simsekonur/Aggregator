#include "AggNode.hpp"

AggNode::AggNode(AggNode* parent, const std::string& name, const std::string& nodeVariableName) {
    this->parent = parent;
    this->name = name;
    this->nodeVariableName = nodeVariableName;
}

std::string AggNode::getVariableName() const {
    return nodeVariableName;
}

std::string AggNode::getName() const {
    return name;
}

void AggNode::setName(const std::string& name) {
    this->name = name;
}

const AggNode* AggNode::getParent() {
    return parent;
}

void AggNode::setParent(const AggNode* parent) {
    this->parent = parent;
}

const std::vector<AggNode*>* AggNode::getChildren() const {
    return &children;
}

void AggNode::setChildren(const std::vector<AggNode*>& children) {
    this->children = children;
}

void AggNode::addChild(AggNode* child) {
    children.push_back(child);
}

