#include "tree.h"
#include <iostream>
#include <QDebug>
int Tree::numTree = 0;
Tree::Tree()
{
    numTree++;
    treeOrderNum = numTree;
}

Tree::Tree(int treeLevel){
    numTree++;
    treeOrderNum = numTree;
    this->treeLevel = treeLevel;
}

int Tree::level(){
    return treeLevel;
}

void Tree::calculateLevel(int level)
{
    qDebug()<<"Calculating "<<treeOrderNum<<". tree's "<<
               level<<". level!";
}

void Tree::readFromDatabase(int level){
    getDatabaseNodes(level);
    qDebug()<<"database nodes are read!";
}

void Tree::getDatabaseNodes(int level){
    qDebug()<<"reading databasenodes of "<<level<<" ";
}
