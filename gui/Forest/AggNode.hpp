#pragma once

#include <vector>
#include <string>

class AggNode {
protected:
    std::vector<AggNode*> children;
    const AggNode* parent;
    std::string name;
    std::string nodeVariableName;
    bool missingData;
public:
    AggNode(AggNode* parent, const std::string& name, const std::string& nodeVariableName);

    virtual bool isLeaf() const = 0;

    std::string getVariableName() const;
    std::string getName() const;
    void setName(const std::string& name);
    const AggNode* getParent();
    void setParent(const AggNode* parent);

    const std::vector<AggNode*>* getChildren() const;
    void setChildren(const std::vector<AggNode*>& children);

    int getRecordCount() const;
    void addChild(AggNode* child);
};
