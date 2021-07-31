#pragma once

#include "AggDataNode.hpp"
#include "AggFormulaNode.hpp"
#include "DBHandler.hpp"
#include "UINode.hpp"
#include <vector>

class AggForest;
class AggTree {
private:
    AggFormulaNode* root;
    const AggForest* forest;
    const AggTree* bluetree;

    std::vector< std::vector<AggFormulaNode*> >  levelList;
    std::vector<AggDataNode*> leafs;
    const DBHandler* dbHandler;
    int id;
    

    void fillLevelList();
    void clear();
    AggNode* createNode(AggNode* parent, UINode* uiNode, int level);

public:
    AggTree();
    ~AggTree();
    AggTree(UINode* uiNode, const DBHandler* handler, int id, const AggTree* bluetree, const AggForest* forest);

    int getId() const;
    void computeLevel(int level);
    const DBHandler* getDBHandler() const;

    void setId(int id);

    AggFormulaNode* getNode(NodeID id) const;

    int getReplicationCount() const;
    const std::vector<AggFormulaNode*>* getLevel(int level) const;

    void fillLeafNodes();

    void print() const;
    void printAsBluetree() const;
    int getLowestFormulaLevel() const;

    UINode* generateUITree(QTreeWidget* widget) const;
    UINode* generateUINode(UINode* parent, AggNode* node) const;

    double getResult();
};
