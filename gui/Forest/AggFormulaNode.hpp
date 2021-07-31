#pragma once
#include "AggNode.hpp"
#include "AggParser.hpp"
#include "UINode.hpp"

struct NodeID {
    int i;
    int j;
    NodeID(): i(0), j(0) {}
    NodeID(int i, int j): i(i), j(j) {}
};

class AggFormulaNode : public AggNode {
private:
    AggParser parser;
    std::string formula;
    bool missingData;
    NodeID id;
    FormulaConfig config;
    std::map<std::string, double> usedWeights;

    bool fillSymbolTable(const AggFormulaNode* blueNode);
    void updateWeight(const std::string& varName, double weight);
public:
    AggFormulaNode(AggNode* parent, const std::string& name, const std::string& formulaStr, const std::string& nodeVariableName);

    bool isLeaf() const;

    void compute(const AggFormulaNode* blueNode);
    double getResult() const;

    std::string getFormula() const;

    void setVariable(const std::string& varName, const std::vector<double>* vectorValue);
    void setVariable(const std::string& varName, double value);
    double getVariable(const std::string& varName) const;
    
    void setWeight(const std::string& varName, double weight);
    double getWeight(const std::string& varName);

    void setFormulaString(const std::string& formula);
    
    void setNodeID(NodeID id);
    NodeID getID() const;

    bool isMissingData() const;

    void printWeights() const;
    void setConfig(const FormulaConfig& config);
    FormulaConfig getConfig();

    std::string getWeightString() const;
};
