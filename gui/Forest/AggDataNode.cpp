#include "AggDataNode.hpp"
#include <algorithm>
#include "AggExceptions.hpp"
#include "AggTree.hpp"

AggDataNode::AggDataNode(
        const AggTree* tree,
        AggNode* parent,
        const std::string& name,
        const std::string& table,
        const std::string& field,
        const std::string& nodeVariableName
    ) : AggNode(parent, name, nodeVariableName), tree(tree) {
        this->table = table;
        this->field = field;
        this->nodeVariableName = nodeVariableName;
        this->missingData = false;
    }

bool AggDataNode::isLeaf() const {
    return true;
}

void AggDataNode::fillData() {
    try {
        data = tree->getDBHandler()->getData(table, field, tree->getId());
        normalize();
    }
    catch (missing_value_exception& e) {
        missingData = true;
    }
}

bool AggDataNode::isMissingData() const {
    return missingData;
}

void AggDataNode::normalize() {
    if (config.isAuto) {
        tree->getDBHandler()->findFieldMinMax(config, this->table, this->field);
    }

    double max = config.max;
    double min = config.min;

    if (max == min) {
        throw node_exception(std::string("Input max and min are equal. Cannot normalize at node:") + name);
    }
    for (uint i = 0; i < data.size(); i++) {
        if (config.higherIsBetter)
            data[i] = (data[i] - min) / (max - min);
        else
            data[i] = 1 - (data[i] - min) / (max - min);

        if (data[i] > 1)
            data[i] = 1;
        if (data[i] < 0)
            data[i] = 0;
    }
}

const std::vector<double>* AggDataNode::getData() {
    return &data;
}

std::string AggDataNode::getTable() const {
    return table;
}

std::string AggDataNode::getField() const {
    return field;
}

int AggDataNode::getRecordCount() const {
    return tree->getDBHandler()->getCount(table);
}

void AggDataNode::setConfig(const DataConfig &config) {
    this->config = config;
}

DataConfig AggDataNode::getConfig() {
    return config;
}
