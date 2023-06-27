#ifndef TREE_H
#define TREE_H
#include <QVector>
#include <algorithm>
#include <QtMath>
#include <QtDebug>
class Graph;
class Tree;
class Node{
    int number;
    int number_parent;
    Node* node_parent;
    QVector<Node*> vec_node_children;
    QVector<int> vec_parents;
    QVector<int> vec_int_children;
    friend class Tree;
    friend class Graph;
};

class Tree
{
    Node root;
    QVector<int> vec_int_node;
    QVector<Node>vec_node_node;
public:
    Tree(int root);
    bool AddNode(int child,int parent);
    friend class Graph;
};

#endif // TREE_H
