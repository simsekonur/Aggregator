#pragma once
#include "AggTree.hpp"
#include "UINode.hpp"
#include "DBHandler.hpp"
#include <QWaitCondition>

class AggForest {
private:
    const DBHandler* handler;
    AggTree* blueprint;
    std::vector<AggTree*> trees;
    int level;
    int treeNumber;
    int finishedTreeCount;

    double sum(const std::vector<double>& arr) const;
    double stdDeviation(const std::vector<double>& arr) const;
    double mean(const std::vector<double>& arr) const;
    void treeProcess(int i, int firstLevel, bool& threadThrowException);
    void dynamicProcess(bool& threadThrowException);
    QMutex mutex;
    QWaitCondition levelDone;
    QWaitCondition newLevel;

public:
    AggForest(UINode* uiRoot, const DBHandler* handler);
    ~AggForest();
    void calculateLevelWeights(int level);
    void calculateNodeWeights(NodeID id);

    void evaluate();

    void printResult() const;

    UINode* getUITree(QTreeWidget* widget, int i) const;
    int getTreeNumber() const;
    double getResult(int index);
    int getLevel() const;
    AggTree* getTree(int i) const;
};
