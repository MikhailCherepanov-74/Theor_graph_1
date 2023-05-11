#ifndef GRAPH_H
#define GRAPH_H
#include<limits>
#include<math.h>
#include<QVector>
#include<QQueue>
#include<QPair>
#include<QString>
#include<string>
#include<QStandardItemModel>
#include<QDebug>
#include<algorithm>
#include "matrix.h"
#include <cstdlib>
#include <ctime>
using namespace std;
class MainWindow;
class Graph
{
    QVector<QVector<int>> list_adjacency;
    //QVector<QVector<int>> matrix_weight;
    QVector<QVector<int>> matrix_adjacency;
    Matrix* matrix_weight = nullptr;
    int amount_node=0;
    pair<int,QVector<QVector<int>>> FindAmountWays(const QVector<QVector<int>>& list_ways,int current_node,int final_node);
public:
    Graph(QString);
    Graph(QStandardItemModel*);
    Graph();
    Graph(int amount_node);
    ~Graph();
    void AddNode();
    void DeleteNode(int number);
    void AddConnection(int number1,int number2,int weight);
    void CreateListAdjacency();
    void PrintListAdjacency();
    int PrintMaxWaysShimbel(int current_node,int final_node);
    int PrintMinWaysShimbel(int current_node,int final_node);
    QVector<int> GetGeometricDistribution(int amount);
    int GetRandomValueFromVecDistribution(QVector<int> vec_distribution);
    QVector<int> GenerateRandomVector(int n, int my_size);
    friend class MainWindow;
    bool FindWaysBFS(int node_begin,int node_end,int& amount,QVector<QVector<int>>& ways);
    //BFS работает для графа без циклов
    //     ////////////////////////2 лаба
    pair<QVector<int>,int> FindWayDijkstra(int node_begin,int node_end,int& iteration_counter);
    //Метод Дейкстры работает для графа без отрицательных весов
    pair<QVector<int>,int> FindBelmanFord(int node_begin,int node_end,int& iteration_counter);
    //Метод Белмана-Форда работает на графах без отрицательных циклов
    pair<QVector<int>,int> FindFloydWarshal(int node_begin,int node_end,int& iteration_counter);
};

#endif // GRAPH_H
