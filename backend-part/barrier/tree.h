#ifndef TREE_H
#define TREE_H

#include "globals.h"


class Tree
{
public:
    Tree();
    Tree(int treeLevel);
    static int numTree;
    int level();
    void virtual calculateLevel(int level);
    void virtual readFromDatabase(int level);
    void virtual getDatabaseNodes(int level);
private:
    int treeOrderNum;
    int treeLevel;

};

#endif // TREE_H
