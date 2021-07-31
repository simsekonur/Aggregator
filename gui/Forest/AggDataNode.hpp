#pragma once

#include "AggNode.hpp"
#include "DBHandler.hpp"
#include "UINode.hpp"

class AggTree;

class AggDataNode : public AggNode {
private:
    std::vector<double> data;
    std::string table;
    std::string field;
    const AggTree* tree;
    DataConfig config;

    void normalize();
public:
    AggDataNode(const AggTree* tree, AggNode* parent, const std::string& name, const std::string& table, const std::string& field, const std::string& nodeVariableName);
    bool isLeaf() const;

    void fillData();
    const std::vector<double>* getData();
    std::string getTable() const;
    std::string getField() const;
    int getRecordCount() const;
    bool isMissingData() const;
    void setConfig(const DataConfig& config);
    DataConfig getConfig();
};
