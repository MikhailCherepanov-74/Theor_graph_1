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
    bool oriented=0;
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
    QPushButton* find_max_flow;
    QPushButton* add_cost_edges;
    QPushButton* find_min_cost_max_flow;
    QPushButton* delete_all_costs;
    QPushButton* change_orient;
    QPushButton* find_prima;
    QPushButton* find_cruskala;
    QPushButton* code_prufer;
    QPushButton* find_ailer_circle;
    QPushButton* build_gamilton;
    QPushButton* find_comivoajor;
    QPushButton* find_amount_ostov;
    QLineEdit* f_node_for_connect;
    QLineEdit* s_node_for_connect;
    QLineEdit* f_node_for_bfs;
    QLineEdit* s_node_for_bfs;
    QLineEdit* amount_node_for_table;
    QLineEdit* weight_for_connecting;
    QLineEdit* le_start_max_flow;
    QLineEdit* le_end_max_flow;
    QTextEdit* list_adjacency;
    QTableView* matrix;
    QVector<NodeItem*> vector_nodes;
    QVector<ConnectionItem*> vector_connections;
    QVector<NodeItem*> vector_selected_nodes;
    QVector<ConnectionItem*> vector_selected_connections;
    QStandardItemModel* model;
    QVector<QVector<int>>ways_for_selected;
    Matrix matrix_cost;
    Matrix matrix_ostov=Matrix(0);
    int number_current_way=-1;
    void PrintVecWay(QVector<int>vec);//help_func
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
    // 3 лаба
    void FindMaxFlow();
    void AddRandomCosts();
    void DeleteAllCosts();
    void FindMinCostsMaxFlow();
    // 4 лаба
    void ChangeOriented();
    void FindPrimaKruskal(int flag); // flag=0 - Прима ; flag=1 - Краскал
    void SelectPairs(QVector<pair<int,int>> vec_pair);
    void CodePrufer();
    Matrix FromVecPairToMatrix(QVector<pair<int,int>> vec);
    int Kirhgof( QVector<QVector<int>>& arr, QVector<QVector<int>>& matrixVesov); // от  Марго
    int findMinor( QVector<QVector<int>>& buffer, int x, int y);                 // от Марго
    void FindAmountOstov();
    friend class ConnectionItem;
    // 5 лаба
    void FindEulerCircle();
    Matrix GraphToEuler( Matrix matrix_weight/*vector<vector<int>>& EulerGr*/);
    bool isEuler(Matrix matrix_weight,QVector<int>&deg);
    void BuildGamilton();
    bool VertixIsAbleForGamilton(QVector<int> v);
    void FindComivoajor();

};

// MAINWINDOW_H
