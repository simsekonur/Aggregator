#include "AggTree.hpp"
#include "AggExceptions.hpp"
#include "AggForest.hpp"
#include <QIcon>

AggTree::AggTree() {
    root = nullptr;
    dbHandler = nullptr;
    id = -1;
}

AggTree::AggTree(UINode* uiNode, const DBHandler* handler, int id, const AggTree* bluetree, const AggForest* forest): dbHandler(handler) {
    root = (AggFormulaNode*) createNode(nullptr, uiNode, 0);
    this->id = id;
    this->bluetree = bluetree;
    this->forest = forest;
}

AggTree::~AggTree() {
    for (auto level: levelList)
        for (auto node: level)
            delete node;
    
    for (auto node: leafs)
        delete node;
}

void AggTree::setId(int id) {
    this->id = id;
}

AggNode* AggTree::createNode(AggNode* parent, UINode* uiNode, int level) {
    if (uiNode->isLeaf()) {
        AggDataNode* dNode = new AggDataNode(
            this,
            parent,
            uiNode->getName(),
            uiNode->getDatabaseTable(),
            uiNode->getDatabaseField(),
            uiNode->getVariableName()
        );
        dNode->setConfig(uiNode->dConfig);
        leafs.push_back(dNode);
        return dNode;
    }
    else {
        AggFormulaNode* fNode = new AggFormulaNode(
            parent,
            uiNode->getName(),
            uiNode->getFormulaString(),
            uiNode->getVariableName()
        );
        fNode->setConfig(uiNode->fConfig);
        // Fill the levelList
        if (levelList.empty() || level > (levelList.size() - 1)) {
            std::vector<AggFormulaNode*> newLevel;
            fNode->setNodeID(NodeID(level, 0));
            newLevel.push_back(fNode);
            levelList.push_back(newLevel);
        }
        else {
            fNode->setNodeID(NodeID(level, levelList[level].size()));
            levelList[level].push_back(fNode);
        }
        const std::vector<UINode*>& uiChildren = *uiNode->getChildren();
        for (UINode* child: uiChildren) {
            AggNode* aggChild = createNode(fNode, child, level+1);
            fNode->addChild(aggChild);
        }

        return fNode;
    }
}

void AggTree::computeLevel(int i) {
    const std::vector<AggFormulaNode*>& iLevel = levelList[i];
    for (AggFormulaNode* node: iLevel) {
        NodeID id = node->getID();
        AggFormulaNode* blueNode = bluetree->getNode(id);
        node->compute(blueNode);
    }
}

const DBHandler* AggTree::getDBHandler() const {
    return dbHandler;
}

int AggTree::getId() const {
    return id;
}

AggFormulaNode* AggTree::getNode(NodeID id) const {
    try {
        return levelList[id.i][id.j];
    }
    catch (...) {
        std::stringstream err;
        err << "No node with id: " << id.i << "," << id.j;
        throw node_exception(err.str());
    }
}

const std::vector<AggFormulaNode*>* AggTree::getLevel(int level) const {
    if (level < levelList.size()) {
        return &levelList[level];
    }
    else {
        std::stringstream err;
        err << "Tree has no level with index " << level;
        throw tree_exception(err.str());
    }
}

int AggTree::getReplicationCount() const {
    if (!leafs.empty()) {
        return leafs[0]->getRecordCount();
    }
    else {
        throw database_exception("Cannot get count of replications from database.");
    }
}

void AggTree::fillLeafNodes() {
    for (AggDataNode* leaf: leafs) {
        leaf->fillData();
    }
}

void AggTree::printAsBluetree() const {
    int i = 0;
    for (auto level: levelList) {
        std::cout << "\tLevel #" << i << ": ";
        for (auto node: level) {
            std::cout << node->getName() << "(";
            node->printWeights();
            std::cout << ") ";
        }
        std::cout << std::endl;
        i++;
    }
}

void AggTree::print() const {
    std::cout << "Tree #" << id << ":" << std::endl;
    int i = 0;
    for (auto level: levelList) {
        std::cout << "\tLevel #" << i << ": ";
        for (auto fNode: levelList[i]) {
            std::cout << fNode->getName() << "=" << fNode->getResult();
            std::cout << "(" << fNode->getWeightString() << ") ";
        }
        std::cout << std::endl;
        i++;
    }
}

int AggTree::getLowestFormulaLevel() const {
    return levelList.size() - 1;
}

UINode* AggTree::generateUITree(QTreeWidget* widget) const {
    UINode* newRoot = new UINode(widget, false, root->getName(), "", root->getFormula());
    const std::vector<AggNode*>& children = *root->getChildren();
    for (auto child: children) {
        UINode* newChild = generateUINode(newRoot, child);
        newRoot->addChild(newChild);
    }
    newRoot->setResult(std::to_string(root->getResult()));
    QString s1(newRoot->getName().c_str());
    QString s2( (newRoot->getFormulaString().c_str()));
    QString s3( newRoot->getVariableName().c_str());
    QString s4( std::to_string(root->getResult()).c_str());
    newRoot->setText(0, s1);
    newRoot->setIcon(0, QIcon(":/icons/root.png"));
    newRoot->setText(2, s2);
    newRoot->setText(1, s3);
    newRoot->setText(4, s4);
    newRoot->fConfig = root->getConfig();
    newRoot->setExpanded(true);

    if (root->isMissingData())
        newRoot->missingData = true;
    newRoot->updateUITexts();

    return newRoot;
}

UINode* AggTree::generateUINode(UINode* parent, AggNode* node) const {
    if (node->isLeaf()) {
        AggDataNode* dataNode = (AggDataNode*) node;
        UINode* newUINode = new UINode(true, dataNode->getName(),
                                       dataNode->getVariableName(),
                                       dataNode->getTable(),
                                       dataNode->getField());
        QString s1(newUINode->getName().c_str());
        QString s2( (newUINode->getDatabaseTable()+"."+newUINode->getDatabaseField()).c_str() );
        QString s3( newUINode->getVariableName().c_str());
        newUINode->setText(0, s1);
        newUINode->setIcon(0, QIcon(":/icons/leaf.png"));
        newUINode->setText(1, s3);
        newUINode->setText(3, s2);

        if (dataNode->isMissingData())
            newUINode->missingData = true;
        newUINode->updateUITexts();
        
        newUINode->dConfig = dataNode->getConfig();
        return newUINode;
    }
    else {
        AggFormulaNode* formulaNode = (AggFormulaNode*) node;
        UINode* newUINode = new UINode(false, formulaNode->getName(),
                                       formulaNode->getVariableName(),
                                       formulaNode->getFormula());

        newUINode->setResult(std::to_string(formulaNode->getResult()));
        QString s1(newUINode->getName().c_str());
        QString s2( (newUINode->getFormulaString().c_str()));
        QString s3( newUINode->getVariableName().c_str());
        QString s4( newUINode->getResult().c_str());
        newUINode->setText(0, s1);
        newUINode->setIcon(0, QIcon(":/icons/branch.png"));
        newUINode->setText(2, s2);
        newUINode->setText(1, s3);
        newUINode->setText(4, s4);

        newUINode->setExpanded(true);

        if (formulaNode->isMissingData())
            newUINode->missingData = true;
        newUINode->updateUITexts();

        newUINode->fConfig = formulaNode->getConfig();
        const std::vector<AggNode*>& children = *node->getChildren();
        for (auto child: children) {
            UINode* newChild = generateUINode(newUINode, child);
            newUINode->addChild(newChild);
        }

        return newUINode;
    }
}

double AggTree::getResult() {
    return root->getResult();
}
