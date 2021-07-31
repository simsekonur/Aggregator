#include "AggForest.hpp"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <QFuture>
#include <QtConcurrent>
#include "AggExceptions.hpp"

class WrapperException : public QtConcurrent::Exception {
public:
    WrapperException(std::exception& err) : e(std::exception(err.what())) {}
    void raise() const { throw *this; }
    QtConcurrent::Exception* clone() const { return new WrapperException(*this); }
    std::exception error() const { return e; }
private:
    std::exception e;
};

#define SQ(x) ((x)*(x))

AggForest::AggForest(UINode* uiRoot, const DBHandler* handler): handler(handler) {
    blueprint = new AggTree(uiRoot, handler, -1, nullptr, this);
    int recordCount = blueprint->getReplicationCount();
    for (int i = 0; i < recordCount; i++) {
        trees.push_back(new AggTree(uiRoot, handler, i, blueprint, this));
    }
    level = blueprint->getLowestFormulaLevel();
    treeNumber = blueprint->getReplicationCount();
}

AggForest::~AggForest() {
    delete blueprint;
    for (auto tree: trees)
        delete tree;
}


double AggForest::sum(const std::vector<double>& arr) const {
    double sum = 0;
    int n = arr.size();
    for (int i = 0; i < n; i++)
        sum += arr[i];
    return sum;
}

double AggForest::mean(const std::vector<double>& arr) const {
    double total = sum(arr);
    return total / arr.size();
}

double AggForest::stdDeviation(const std::vector<double>& arr) const {
    double avg = mean(arr);
    double variance = 0;
    int n = arr.size();
    for (int i = 0; i < n; i++)
        variance += SQ(arr[i] - avg);
    return sqrt(variance / n);
}

void AggForest::calculateNodeWeights(NodeID id) {
    AggFormulaNode* mainNode = blueprint->getNode(id);
    const std::vector<AggNode*>& allChildren = *mainNode->getChildren();
    std::vector<AggFormulaNode*> children;

    // filter and take only formula nodes
    for (const auto& child: allChildren) {
        if (!child->isLeaf()) {
            children.push_back((AggFormulaNode*) child);
        }
    }

    int childrenCount = children.size();
    int siblingCount = childrenCount - 1;

    if (childrenCount == 1) {
        AggFormulaNode* w_child = children[0];
        NodeID id = w_child->getID();
        mainNode->setVariable(w_child->getVariableName(), 1);
        return;
    }

    double denominator = 0;
    std::vector<double> preWeights;
    preWeights.resize(childrenCount);
    for (int w_child_index = 0; w_child_index < childrenCount; w_child_index++) { // for every w_child
        AggFormulaNode* w_child = (AggFormulaNode*) children[w_child_index];
        NodeID w_id = w_child->getID();

        std::vector<double> correlations;

        // initialize result arrays. How many formula node childs does the main node have
        correlations.resize(siblingCount);

        bool w_child_skipped = false;
        for (int siblingIndex = 0; siblingIndex < childrenCount; siblingIndex++) {
            if (siblingIndex == w_child_index) {
                w_child_skipped = true;
                continue;
            }
            AggFormulaNode* siblingNode = children[siblingIndex];
            NodeID siblingID = siblingNode->getID();
            std::vector<double> w_child_results;
            double w_child_std;
            double w_child_mean;

            std::vector<double> siblingResults;
            double siblingMean;
            double siblingStd;

            // Fill the results.
            for (int treeIndex = 0; treeIndex < treeNumber; treeIndex++) {
                const AggTree& tree = *trees[treeIndex];

                // If the w child or the sibling has missing data in this tree, skip this tree.
                if (tree.getNode(w_id)->isMissingData() || tree.getNode(siblingID)->isMissingData())
                    continue;

                w_child_results.push_back(tree.getNode(w_id)->getResult());
                siblingResults.push_back(tree.getNode(siblingID)->getResult());
            }

            w_child_mean = mean(w_child_results);
            siblingMean = mean(siblingResults);
            w_child_std = stdDeviation(w_child_results);
            siblingStd = stdDeviation(siblingResults);

            double r_w_sibling = 0;
            int nonMissingCount = w_child_results.size();
            for (int i = 0; i < nonMissingCount; i++) {
                r_w_sibling +=
                    ( (w_child_results[i] - w_child_mean) / w_child_std )
                    *
                    ( (siblingResults[i] - siblingMean) / siblingStd );
            }

            if (w_child_skipped) // if w child skipped use -1 on index since children array is one length shorter than allChildren
                correlations[siblingIndex - 1] = r_w_sibling / (nonMissingCount - 1);
            else
                correlations[siblingIndex] = r_w_sibling / (nonMissingCount - 1);

        }
        // add the sum of (1-r_ax) to denominator
        double correlationSum = std::accumulate(correlations.begin(), correlations.end(), 0.0, [] (double a, double b) {return a + b;});
        correlationSum = siblingCount - correlationSum;
        denominator += correlationSum;
        preWeights[w_child_index] += correlationSum;
    }

    // calculate w_x's and insert into blueprint node
    for (int i = 0; i < childrenCount; i++) {
        AggFormulaNode* w_child = children[i];
        NodeID id = w_child->getID();
        mainNode->setVariable(w_child->getVariableName(), preWeights[i] / denominator);
    }
}

void AggForest::calculateLevelWeights(int level) {
    const std::vector<AggFormulaNode*>& levelNodes = *(blueprint->getLevel(level));
    const int size = levelNodes.size();
    for (int i = 0; i < size; i++) { // i'th child
        try {
            calculateNodeWeights(levelNodes[i]->getID());
        }
        catch (...) {
            throw node_exception("Error while calculating weights of children for node: " + levelNodes[i]->getName());
        }
    }
}

void AggForest::evaluate() {
    level = blueprint->getLowestFormulaLevel();
    level--;

    QVector<QFuture<void>> treeWorkers;
    QThreadPool::globalInstance()->setMaxThreadCount(treeNumber + 1);

    bool threadThrowException = false;

    QFuture<void> dynamicWorker = QtConcurrent::run(this, &AggForest::dynamicProcess, threadThrowException);
    for (AggTree* tree: trees) {
        int id = tree->getId();
        treeWorkers.append(QtConcurrent::run(this, &AggForest::treeProcess, id, level+1, threadThrowException));
    }

    std::exception e;
    bool caught = false;
    for (auto& worker: treeWorkers) {
        try {
            worker.waitForFinished();
        }
        catch (WrapperException& we) {
            if (!caught) {
                e = we.error();
                caught = true;
            }
        }
        catch (QtConcurrent::UnhandledException& ue) {
            if (!caught) {
                e = std::exception("Unknown QtConcurrent exception in tree thread.");
                caught = true;
            }
        }
    }
    if (caught)
        std::cout << "BEFORE DYNAMIC ENDS CAUGHT FROM TREES: " << e.what() << std::endl;
    try {
        dynamicWorker.waitForFinished();
    }
    catch (WrapperException& we) {
        if (!caught) {
            e = we.error();
            caught = true;
        }
    }
    catch (QtConcurrent::UnhandledException& ue) {
        if (!caught) {
            e = std::exception("Unknown QtConcurrent exception in dynamic parameter thread.");
            caught = true;
        }
    }

    if (caught) {
        std::cout << "CAUGHT THE EXCEPTION IN evaluate()" << std::endl;
        throw e;
    }
}

void AggForest::treeProcess(int i, int firstLevel, bool& oneThreadThrew) {
    AggTree* tree = trees[i];

    auto cleanAndThrow = [&] (std::exception& ex) {
        mutex.lock();
        if (!oneThreadThrew) {
            oneThreadThrew = true;
            finishedTreeCount = finishedTreeCount - 1;
            mutex.unlock();
            levelDone.wakeOne();
            std::cout << ex.what() << std::endl;
            throw WrapperException(ex);
        }
        mutex.unlock();
    };

    auto compute = [&] (int curLevel, bool isFirstLevel) -> bool {
        try {
            if (isFirstLevel)
                tree->fillLeafNodes();
            tree->computeLevel(curLevel); // heavy independent work. so lock is unlocked before
            return true;
        }
        catch (std::exception& ex) {
            cleanAndThrow(ex);
            return false;
        }
        catch (...) {
            std::exception e = tree_exception("Unknown error in tree thread #" + std::to_string(i));
            cleanAndThrow(e);
            return false;
        }
    };

    if (!compute(firstLevel, true))
        return;

    mutex.lock();
    finishedTreeCount = finishedTreeCount - 1;
    if (finishedTreeCount == 0) {
        levelDone.wakeOne();
    }

    while (level >= 0) {
        newLevel.wait(&mutex);
        if (oneThreadThrew) {
            mutex.unlock();
            return;
        }
        mutex.unlock();

        if (!compute(level, false))
            return;

        mutex.lock();
        finishedTreeCount = finishedTreeCount - 1;
        if (finishedTreeCount == 0) {
            level = level - 1;
            levelDone.wakeOne();
            if (level < 0) {
                mutex.unlock();
                return;
            }
        }
        else {
            if (level == 0) {
                mutex.unlock();
                return;
            }
        }
    }
}

void AggForest::dynamicProcess(bool& oneThreadThrew) {
    mutex.lock();
    finishedTreeCount = treeNumber;

    levelDone.wait(&mutex);
    if (oneThreadThrew) {
        mutex.unlock();
        return;
    }

    while (level >= 0) {
        finishedTreeCount = treeNumber;

        try {
            calculateLevelWeights(level);
        }
        catch (std::exception& ex) {
            if (!oneThreadThrew) {
                oneThreadThrew = true;
                newLevel.wakeAll();
                mutex.unlock();
                throw WrapperException(ex);
            }
            mutex.unlock();
            return;
        }
        catch (...) {
            if (!oneThreadThrew) {
                oneThreadThrew = true;
                newLevel.wakeAll();
                mutex.unlock();
                std::exception e = tree_exception("Unknown error in dynamic parameter thread.");
                throw WrapperException(e);
            }
            mutex.unlock();
            return;
        }

        newLevel.wakeAll();
        levelDone.wait(&mutex);
        if (oneThreadThrew) {
            mutex.unlock();
            return;
        }
    }
    mutex.unlock();
}

void AggForest::printResult() const {
    std::cout << "Blueprint weights: " << std::endl;
    blueprint->printAsBluetree();

    for (auto tree: trees) {
        tree->print();
    }
}

UINode* AggForest::getUITree(QTreeWidget* widget, int i) const {
    return trees[i-1]->generateUITree(widget);
}

int AggForest::getTreeNumber() const {
    return treeNumber;
}

double AggForest::getResult(int index) {
    return trees[index]->getResult();
}

int AggForest::getLevel() const {
    return level;
}

AggTree* AggForest::getTree(int i) const {
    return trees[i];
}
