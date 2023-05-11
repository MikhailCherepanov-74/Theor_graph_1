#pragma once
#include "Shape.h"
#include "graph.h"
#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include <vector>
#include <QVector>
#include <QPushButton>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <algorithm>
#include <QStandardItemModel>
#include <QTableView>
#include <QTextEdit>
#include <QDebug>
#include <cstdlib>
#include <ctime>
#include <QtMath>
using namespace std;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void ChangeZValue();
    void FillGeneralVectors(QVector<QVector<int>>matrix, int amount);
    void DrawGraph();
    void GetPosition(int& x,int& y);
    void LoadTable();
private:
    Graph* general_graph;
    Ui::MainWindow *ui;
    QGraphicsScene* scene;
    QPushButton* add_node;
    QPushButton* add_connection;
    QPushButton* delete_node;
    QPushButton* set_from_table;
    QPushButton* set_from_list;
    QPushButton* reset_table;
    QPushButton* find_ways_bfs;
    QPushButton* show_ways;
    QPushButton* show_prev_way;
    QPushButton* show_next_way;
    QPushButton* deselect_all;
    QPushButton* find_way_dijkstra;
    QPushButton* find_way_belman_ford;
    QPushButton* find_way_floyd_warshall;
    QLineEdit* f_node_for_connect;
    QLineEdit* s_node_for_connect;
    QLineEdit* f_node_for_bfs;
    QLineEdit* s_node_for_bfs;
    QLineEdit* amount_node_for_table;
    QLineEdit* weight_for_connecting;
    QTextEdit* list_adjacency;
    QTableView* matrix;
    QVector<NodeItem*> vector_nodes;
    QVector<ConnectionItem*> vector_connections;
    QVector<NodeItem*> vector_selected_nodes;
    QVector<ConnectionItem*> vector_selected_connections;
    QStandardItemModel* model;
    QVector<QVector<int>>ways_for_selected;
    int number_current_way=-1;
private slots:
    void AddNode();
    void DeleteNode();
    void AddConection();
    void SetFromList(){
        QString str=list_adjacency->toPlainText();
        if(general_graph!=NULL)
            delete general_graph;
        general_graph=new Graph(str);
    }
    void SetFromTable();
    void ResetTable(int amount);
    void FindWaysBFS();
    void SelectWay(QVector<int> vec_for_selected);
    void DeselectAll();
    void FindWayDijkstra_BelmanFord_FloydWorshal(int flag);//flag=0 - Dijkstra, flag=1 - BelmanFord,flag=2 - Floyd Worshal
   // void FindWayBelmanFord();
    friend class ConnectionItem;
};

// MAINWINDOW_H
