#include <QCoreApplication>
#include <iostream>
#include <vector>
#include "tree.h"
#include "forest.h"


using namespace std;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    vector<Tree> trees;
    for(int i=0; i<20;i++){
        trees.push_back(*(new Tree(3)));
    }
    Forest myForest(3, trees);
    myForest.calculate();
    return a.exec();
}
