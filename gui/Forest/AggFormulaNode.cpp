#include "AggFormulaNode.hpp"
#include "AggDataNode.hpp"
#include "AggExceptions.hpp"

AggFormulaNode::AggFormulaNode(AggNode* parent, const std::string& name, const std::string& formula, const std::string& nodeVariableName)
    : AggNode(parent, name, nodeVariableName), formula(formula) , missingData(false) {}

bool AggFormulaNode::isLeaf() const {
    return false;
}

void AggFormulaNode::compute(const AggFormulaNode* blueNode) {
    try {
        bool thisNodeNotMissing = fillSymbolTable(blueNode);
        if (thisNodeNotMissing) {
            parser.parse(formula);
            double res = parser.getResult();
            if (res < 0 || res > 1)
                throw node_exception("Result value out of [0,1] range. (result: " + std::to_string(res) + ")");
        }
        else
            parser.setResult(0.0);
    }
    catch (std::exception& e) {
        throw std::exception((e.what() + std::string(" at node: ") + name).c_str());
    }

    //double result = parser.getResult();
    //if (result < 0 || result > 1) {
    //    throw node_exception("Out of boundary results in node: " + name + ". Computed result = " + std::to_string(result));
    //}
}

double AggFormulaNode::getResult() const {
    return parser.getResult();
}

std::string AggFormulaNode::getFormula() const {
    return formula;
}

void AggFormulaNode::setFormulaString(const std::string& formula) {
    this->formula = formula;
}

void AggFormulaNode::setVariable(const std::string& varName, double value) {
    parser.setVariable(varName, value);
}

double AggFormulaNode::getVariable(const std::string& varName) const {
    return parser.getNumberVariable(varName);
}

void AggFormulaNode::setVariable(const std::string& varName, const std::vector<double>* vectorValue) {
    parser.setVariable(varName, vectorValue);
}

void AggFormulaNode::updateWeight(const std::string& name, double weight) {
    auto it = usedWeights.find(name);
    auto itUI = config.autoWeights.find(name);
    auto& pairUI = *itUI;
    pairUI.second = weight;
    auto& pair = *it;
    pair.second = weight;
}

bool AggFormulaNode::fillSymbolTable(const AggFormulaNode* blueNode) {
    int missingDataCount = 0;
    int formulaChildCount = 0;
    double missingWeight = 0;
    for (AggNode* node: children) {
        if (node->isLeaf()) {
            AggDataNode* dataNode = (AggDataNode*) node;
            if (dataNode->isMissingData()) {
                missingData = true;
                return false;
            }
            setVariable(node->getVariableName(), dataNode->getData());
        }
        else {
            formulaChildCount++;
            // TODO if the node has missing data make other children share the weights
            AggFormulaNode* formulaNode = (AggFormulaNode*) node;
            const std::string& varName = formulaNode->getVariableName();
            double weight = blueNode->getVariable(varName);
            if (formulaNode->isMissingData()) {
                missingDataCount++;
                setVariable(varName, 0.0);
                usedWeights.emplace(varName, 0.0);
                config.autoWeights.emplace(varName, 0.0);
                std::cout << "adding " << weight << " to missingWeight" << std::endl;
                missingWeight += weight;
            }
            else {
                config.autoWeights.emplace(varName, weight);
                usedWeights.emplace(varName, weight);
                if (config.isAutoWeights) {
                    setVariable(varName, formulaNode->getResult() * weight);
                }
                else {
                    double finalWeight = config.manuelWeights[varName] * config.alpha + weight * (1 - config.alpha);
                    setVariable(varName, formulaNode->getResult() * finalWeight);
                }
            }
        }
    }

    if (missingDataCount == children.size()) {
        missingData = true;
        return false; // this node is now missing data because all children are missing. return false
    }
    if (missingDataCount > 0) {
        double extraPiece = missingWeight / (formulaChildCount - missingDataCount);
        for (AggNode* node: children) {
            if (!node->isLeaf()) {
                const std::string& varName = node->getVariableName();
                double oldWeight = usedWeights[varName];
                if (oldWeight != 0) {
                    double finalWeight = oldWeight + extraPiece;
                    std::cout << "At node: " << name << " this formula node's weight will increase " << extraPiece << std::endl;
                    std::cout << "At node: " << name << " final weight: " << finalWeight << std::endl;
                    updateWeight(varName, finalWeight);
                    if ( !config.isAutoWeights) {
                        try {
                            finalWeight = (1 - config.alpha) * finalWeight + config.alpha * config.manuelWeights[varName];
                        }
                        catch (...) {
                            throw node_exception("Failed while calculating hybrid weight with alpha, at node: " + name);
                        }
                    }
                    parser.updateVariable(varName, getVariable(varName) * finalWeight);
                }
            }
        }
    }
    return true;
}

bool AggFormulaNode::isMissingData() const {
    return missingData;
}

NodeID AggFormulaNode::getID() const {
    return id;
}

void AggFormulaNode::setNodeID(NodeID id) {
    this->id = id;
}

void AggFormulaNode::printWeights() const {
    parser.printWeights();
}

void AggFormulaNode::setConfig(const FormulaConfig &config) {
    this->config = config;
}

FormulaConfig AggFormulaNode::getConfig() {
    return config;
}

std::string AggFormulaNode::getWeightString() const {
    std::stringstream stream;
    for (const auto& pair: usedWeights) {
        stream << "w_" << pair.first << "=" << pair.second << " ";
    }
    return stream.str();
}
