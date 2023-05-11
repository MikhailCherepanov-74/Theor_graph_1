#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    std::srand(std::time(0));
    ui->setupUi(this);
    general_graph=new Graph();
    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    this->add_node = ui->add_node;
    this->add_connection = ui->add_connection;
    this->delete_node = ui->delete_node;
    this->f_node_for_connect=ui->f_node_for_connect;
    this->s_node_for_connect=ui->s_node_for_connect;
    this->matrix=ui->tableView;
    this->set_from_table= ui->set_from_table;
   // this->set_from_list= ui->set_from_list;
   // this->list_adjacency=ui->list_adjacency;
    this->reset_table=ui->reset_table;
    this->amount_node_for_table=ui->amount_node_for_table;
    this->weight_for_connecting=ui->weight_le;
    this->find_ways_bfs=ui->find_ways_bfs;
    this->f_node_for_bfs=ui->bfs_1;
    this->s_node_for_bfs=ui->bfs_2;
    this->show_ways=ui->shiw_ways;
    this->show_prev_way=ui->show_prev_way;
    this->show_next_way=ui->show_next_way;
    this->deselect_all=ui->deselect_all;
    this->find_way_dijkstra=ui->find_way_dijkstra;
    this->find_way_belman_ford=ui->find_way_belman_ford;
    this->find_way_floyd_warshall=ui->find_way_floyd_warshall;
    connect(add_node, SIGNAL(clicked()), this, SLOT(AddNode()));
    connect(delete_node, SIGNAL(clicked()), this, SLOT(DeleteNode()));
    connect(add_connection,SIGNAL(clicked()), this, SLOT(AddConection()));
   // connect(set_from_list,SIGNAL(clicked()), this, SLOT(SetFromList()));
    connect(set_from_table,SIGNAL(clicked()), this, SLOT(SetFromTable()));
    connect(reset_table,&QPushButton::clicked,[this](){
        ResetTable(amount_node_for_table->text().toInt());
    });
    connect(find_ways_bfs,&QPushButton::clicked,[this](){
        FindWaysBFS();

    });
    connect(find_way_dijkstra,&QPushButton::clicked,[this](){
        FindWayDijkstra_BelmanFord_FloydWorshal(0);
    });
    connect(find_way_belman_ford,&QPushButton::clicked,[this](){
        FindWayDijkstra_BelmanFord_FloydWorshal(1);
    });
    connect(find_way_floyd_warshall,&QPushButton::clicked,[this](){
        FindWayDijkstra_BelmanFord_FloydWorshal(2);
    });
    connect(show_ways,&QPushButton::clicked,[this](){
        if(ways_for_selected.empty())
            qDebug()<<"Error selected";
        else{
            SelectWay(ways_for_selected[0]);
            number_current_way=0;
        }
    });
    connect(show_prev_way,&QPushButton::clicked,[this](){
        if(number_current_way<1)
            qDebug()<<"Nothing else";
        else{
            SelectWay(ways_for_selected[number_current_way-1]);
            number_current_way--;
        }
    });
    connect(show_next_way,&QPushButton::clicked,[this](){
     if(number_current_way!=-1&&number_current_way<ways_for_selected.size()-1){
            SelectWay(ways_for_selected[number_current_way+1]);
            number_current_way++;
     }
     else
            qDebug()<<"Nothing else";
    });
    connect(deselect_all,&QPushButton::clicked,[this](){
        DeselectAll();
        number_current_way=-1;
    });

}

void MainWindow::AddNode() {
    general_graph->AddNode();
    //FillGeneralVectors(general_graph->matrix_graph,general_graph->amount_node);
    //DrawGraph();
    NodeItem* node1 = new NodeItem(vector_nodes.size()+1,0,0,scene);
    int x,y;
    GetPosition(x,y);
    node1->setPos(x,y);
    vector_nodes.push_back(node1);
    scene->addItem(vector_nodes.back());
    ChangeZValue();
    LoadTable();
    DeselectAll();
    number_current_way=-1;
    ways_for_selected.clear();
}
void  MainWindow::DeleteNode() {
      DeselectAll();
    int max;
    int i_max=0;
    NodeItem* forward_shape;
    if (!vector_nodes.isEmpty()) {
        forward_shape = vector_nodes.back();
        max=vector_nodes[0]->zValue();
        for (int i=0; i< vector_nodes.size(); i++)
            if (max <= vector_nodes[i]->zValue())
            {
                max = vector_nodes[i]->zValue();
                forward_shape = vector_nodes[i];
                i_max=i;
            }

        scene->removeItem(forward_shape);

        for_each(vector_nodes.begin()+i_max+1,vector_nodes.end(),[this](NodeItem* a){
             scene->removeItem(a);
             a->NumberMinusOne();
             this->scene->addItem(a);
        });



        vector_nodes.erase(vector_nodes.begin()+i_max);



        for(int i=0;i<vector_nodes.size();i++){
            for(int j=0;j<forward_shape->vector_connection.size();j++){
            vector_nodes[i]->vector_connection.removeAll(forward_shape->vector_connection[j]);
            }
        }


        for(int i=0;i<forward_shape->vector_connection.size();i++){
            vector_connections.removeAll(forward_shape->vector_connection[i]);
            scene->removeItem(forward_shape->vector_connection[i]->line);
            scene->removeItem(forward_shape->vector_connection[i]->polygon_triangle);
            scene->removeItem(forward_shape->vector_connection[i]->text_weght);

            delete forward_shape->vector_connection[i];
        }
        delete forward_shape;
        general_graph->DeleteNode(i_max+1);
    }
    ChangeZValue();
    LoadTable();
    DeselectAll();
    number_current_way=-1;
    ways_for_selected.clear();
}
void MainWindow::AddConection(){
    bool normal_connect;
    bool normal_weight;
    int f_num=f_node_for_connect->text().toInt(&normal_connect);
    int s_num=s_node_for_connect->text().toInt(&normal_connect);
    int weight=weight_for_connecting->text().toInt(&normal_weight);
    if(!normal_weight||weight==0)
        weight=1;
    if(normal_connect&&f_num<=vector_nodes.size()&&s_num<=vector_nodes.size()){
    ConnectionItem *connection =new ConnectionItem(vector_nodes[f_num-1],vector_nodes[s_num-1],weight,scene);
    vector_nodes[f_num-1]->AddConnection(connection);
    vector_nodes[s_num-1]->AddConnection(connection);
    scene->addItem(connection->line);
    scene->addItem(connection->polygon_triangle);
    scene->addItem(connection->text_weght);
    general_graph->AddConnection(f_num,s_num,weight);
    }
    if (model) {
        QModelIndex index = model->index(f_num-1, s_num-1);
        model->setData(index, weight);
    }
    matrix->setModel(model);
    DeselectAll();
    number_current_way=-1;
    ways_for_selected.clear();
}

void MainWindow::ResetTable(int amount){
    if(model!=nullptr)
        delete model;
    if(general_graph!=NULL){
        DeselectAll();
        number_current_way=-1;
        ways_for_selected.clear();
        delete general_graph;
    }
    general_graph=new Graph(amount);

    model=new QStandardItemModel(amount, amount);
    for(int i=0;i<amount;i++){
    model->setHeaderData(i, Qt::Horizontal,  QString::number(i+1)); // устанавливаем заголовок для первого столбца
    model->setHeaderData(i, Qt::Vertical,  QString::number(i+1)); // устанавливаем заголовок для первого столбца
    }/*
    for(int i=0;i<amount;i++){
        for(int j=0;j<amount;j++){
             model->setItem(i, j, new QStandardItem("0"));
        }
    }*/
  //  qDebug()<<"Random";
  //  general_graph->matrix_weight->Print();

    for(int i=0;i<amount;i++){
        for(int j=0;j<amount;j++){
             model->setItem(i, j, new QStandardItem(QString::number(general_graph->matrix_weight->data[i+1][j+1])));
        }
    }

    matrix->setModel(model);
    matrix->resizeColumnsToContents();
}
void MainWindow::SetFromTable(){
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(matrix->model()); // получение указателя на модель данных

    if (model) {
     /*   vector_nodes.removeAll(nullptr);
        for_each(vector_nodes.begin(),vector_nodes.end(),[](NodeItem* a){
            a->vector_connection.removeAll(nullptr);
        });*/
        if(general_graph!=NULL){
            DeselectAll();
            number_current_way=-1;
            ways_for_selected.clear();
            vector_nodes.clear();
            vector_connections.clear();
            scene->clear();
            delete general_graph;
        }

       general_graph=new Graph(model);

       FillGeneralVectors(general_graph->matrix_weight->data,general_graph->amount_node);
       DrawGraph();
       DeselectAll();
       number_current_way=-1;
       ways_for_selected.clear();
       {
      // general_graph->CreateListAdjacency();
      //general_graph->PrintListAdjacency();

       }
    }
    else {
       qDebug()<<"Table error";
    }
}

void MainWindow::FindWaysBFS(){
    DeselectAll();
    int amount;
    QVector<QVector<int>> ways;
    int my_begin=(this->f_node_for_bfs->text()).toInt();
    int my_end=(this->s_node_for_bfs->text()).toInt();
    qDebug()<<" Way is able?"<< general_graph->FindWaysBFS(my_begin,my_end,amount,ways);
    qDebug()<<"Amount ways"<<amount;
    int counter=0;
    for_each(ways.begin(),ways.end(),[&counter](QVector<int> a){
        QString str=QString::number(counter)+": ";
        for_each(a.begin(),a.end(),[&str](int v){
           str+=QString::number(v)+=" ";
        });
        qDebug()<<str;
        str="";
        counter++;
    });
    ways_for_selected=ways;
}
void MainWindow::FillGeneralVectors(QVector<QVector<int>>matrix, int amount){
    vector_nodes.clear();
    vector_connections.clear();
    for(int i=1;i<amount+1;i++){
        NodeItem* node1 = new NodeItem(vector_nodes.size()+1,0,0,scene);
        int x,y;
        GetPosition(x,y);
        node1->setPos(x,y);
        vector_nodes.push_back(node1);
    }
     for(int i=1;i<amount+1;i++){
        for(int j=1;j<amount+1;j++){
            if(matrix[i][j]!=0){
                ConnectionItem* con_it=new ConnectionItem(vector_nodes[i-1],vector_nodes[j-1],matrix[i][j],scene);
            vector_connections.push_back(con_it);
            vector_nodes[i-1]->AddConnection(vector_connections.back());
            vector_nodes[j-1]->AddConnection(vector_connections.back());
            }
        }
     }
}

void MainWindow::DrawGraph(){
    scene->clear();
    for(int i=0;i<vector_nodes.size();i++)
         scene->addItem(vector_nodes[i]);
    for(int i=0;i<vector_connections.size();i++){
         scene->addItem(vector_connections[i]->line);
         scene->addItem(vector_connections[i]->polygon_triangle);
         scene->addItem(vector_connections[i]->text_weght);
    }
}
void MainWindow::LoadTable(){
    if(model!=nullptr)
        delete model;
    int amount=general_graph->amount_node;
    model=new QStandardItemModel(amount, amount);
    for(int i=0;i<amount;i++){
    model->setHeaderData(i, Qt::Horizontal,  QString::number(i+1)); // устанавливаем заголовок для первого столбца
    model->setHeaderData(i, Qt::Vertical,  QString::number(i+1)); // устанавливаем заголовок для первого столбца
    }
    for(int i=0;i<amount;i++){
        for(int j=0;j<amount;j++){
             model->setItem(i, j, new QStandardItem(QString::number(general_graph->matrix_weight->data[i+1][j+1])));
        }
    }
    matrix->setModel(model);
    matrix->resizeColumnsToContents();
}
MainWindow::~MainWindow()
{
    delete ui;
    delete general_graph;
}
void MainWindow::ChangeZValue() {
    QVector <NodeItem*> ::iterator it2 = vector_nodes.begin();
    for (; it2 != vector_nodes.end(); it2++)
        (*it2)->setZValue(0);
    (*it2)->ZValueToZero();
}
void MainWindow::GetPosition(int& x,int& y){
    static int counter=0;
    counter++;
 //   srand(counter);
    x = (rand() % 600)-300;
    y = (rand() % 600)-300;

}
void MainWindow::SelectWay(QVector<int> vec_for_selected){ // /////////////////////// вот тут точно надо выкинуть исключение
    if(vec_for_selected.empty()){
        qDebug()<<"Error selected";
        return;
    }

   DeselectAll();
    for(int i=0;i<vec_for_selected.size()-1;i++){
        vector_nodes[vec_for_selected[i]-1]->Select();
        vector_selected_nodes.push_back(vector_nodes[vec_for_selected[i]-1]);
        for(int j=0;j<vector_nodes[vec_for_selected[i]-1]->vector_connection.size();j++){
            if(vector_nodes[vec_for_selected[i]-1]->vector_connection[j]->node2== vector_nodes[vec_for_selected[i+1]-1]){
                vector_nodes[vec_for_selected[i]-1]->vector_connection[j]->Select();
                vector_selected_connections.push_back(vector_nodes[vec_for_selected[i]-1]->vector_connection[j]);
            }
        }
    }
    vector_nodes[vec_for_selected[vec_for_selected.size()-1]-1]->Select();
    vector_selected_nodes.push_back(
                vector_nodes[vec_for_selected[vec_for_selected.size()-1]-1]);
}
void  MainWindow::DeselectAll(){
    for(int i=0;i<vector_selected_nodes.size();i++)
        vector_selected_nodes[i]->Deselect();
    for(int i=0;i<vector_selected_connections.size();i++)
        vector_selected_connections[i]->Deselect();
    vector_selected_nodes.clear();
    vector_selected_connections.clear();
  //  number_current_way=-1;
}
//                 ////////////////////////////////////// 2 лаба
void MainWindow::FindWayDijkstra_BelmanFord_FloydWorshal(int flag){
    DeselectAll();
    int my_begin=(this->f_node_for_bfs->text()).toInt();
    int my_end=(this->s_node_for_bfs->text()).toInt();
    int iteration_counter;
    if(my_begin<1||my_end<1||my_begin>general_graph->amount_node||my_end>general_graph->amount_node){
        qDebug()<<"Search error";
        return;
    }
    pair<QVector<int>,int> returned;
    if(flag==0){
    returned=general_graph->FindWayDijkstra(my_begin,my_end,iteration_counter);
    qDebug()<<"Dijkstra Method:";
    }
    if(flag==1){
    returned=general_graph->FindBelmanFord(my_begin,my_end,iteration_counter);
    qDebug()<<"Belman-Ford Method:";
    }
    if(flag==2){
    returned=general_graph->FindFloydWarshal(my_begin,my_end,iteration_counter);
    qDebug()<<"Floyd-Warshall Method:";
    }
    if(returned.second!=INT_MAX){
        qDebug()<<"Min way: "<<returned.second;
        QString str="";
        for(int i=0;i<returned.first.size();i++){
            if(i!=0)
                str+=" ";
           str+=QString::number(returned.first[i]);
        }
         qDebug()<<str;
         qDebug()<<"Iteration counter: "<<iteration_counter;
         ways_for_selected.clear();
         DeselectAll();
         ways_for_selected.push_back(returned.first);

    }
    else{
        qDebug()<<"No ways!";
        qDebug()<<"Iteration counter: "<<iteration_counter;
    }

}
