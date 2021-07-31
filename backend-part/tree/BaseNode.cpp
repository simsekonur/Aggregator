#include "BaseNode.hpp"

BaseNode::BaseNode() {}

const std::vector<BaseNode*>& BaseNode::getChildren() const {
    return children;
}

void BaseNode::addChild(BaseNode* child) {
    children.push_back(child);
}
