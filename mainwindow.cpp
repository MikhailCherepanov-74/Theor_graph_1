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
   // this->le_end_max_flow=ui->v_end_max_flow;
   // this->le_start_max_flow=ui->v_start_max_flow;
    this->find_max_flow=ui->find_max_flow;
    this->add_cost_edges=ui->add_random_cost_edge;
    this->find_min_cost_max_flow=ui->find_min_cost_max_flow;
    this->delete_all_costs=ui->delete_all_costs;
    this->change_orient=ui->change_orient;
    this->find_prima=ui->find_prima;
    this->find_cruskala=ui->find_cruskala;
    this->code_prufer=ui->code_prufer;
    this->find_ailer_circle=ui->find_ailer_circle;
    this->build_gamilton=ui->build_gamilton;
    this->find_comivoajor=ui->find_comivoajor;
    this->find_amount_ostov=ui->find_amount_ostov;
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
    connect(find_max_flow,&QPushButton::clicked,[this](){
        FindMaxFlow();
    });
    connect(add_cost_edges,&QPushButton::clicked,[this](){
        AddRandomCosts();
    });
    connect(find_min_cost_max_flow,&QPushButton::clicked,[this](){
        FindMinCostsMaxFlow();
    });
    connect(delete_all_costs,&QPushButton::clicked,[this](){
        DeleteAllCosts();
    });
    connect(change_orient,&QPushButton::clicked,[this](){
        ChangeOriented();
    });
    connect(find_prima,&QPushButton::clicked,[this](){
       FindPrimaKruskal(0);
    });
    connect(find_cruskala,&QPushButton::clicked,[this](){
       FindPrimaKruskal(1);
    });
    connect(code_prufer,&QPushButton::clicked,[this](){
       CodePrufer();
    });
    connect(find_ailer_circle,&QPushButton::clicked,[this](){
       FindEulerCircle();
    });
    connect(build_gamilton,&QPushButton::clicked,[this](){
       BuildGamilton();
    });
    connect(find_comivoajor,&QPushButton::clicked,[this](){
       FindComivoajor();
    });
    connect(find_amount_ostov,&QPushButton::clicked,[this](){
       FindAmountOstov();
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
    DeleteAllCosts();
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
      DeleteAllCosts();
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
    DeleteAllCosts();
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
    vector_connections.push_back(connection);
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
    oriented=0;
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
    DeleteAllCosts();
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
    DeleteAllCosts();
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
// ////////////////////////////////////////////////////////// 3 лаба
void MainWindow::FindAmountOstov(){
    QVector<QVector<int>> arr(general_graph->amount_node,
                              QVector<int>(general_graph->amount_node,0));
    for(int i=1;i<general_graph->amount_node+1;i++){
        for(int j=1;j<general_graph->amount_node+1;j++){
            arr[i-1][j-1]=general_graph->matrix_weight->data[i][j];
        }
    }
    QVector<QVector<int>> matrixVesov=arr;
    for(int i=0;i<general_graph->amount_node;i++){
        for(int j=0;j<general_graph->amount_node;j++){
            if(arr[i][j]!=0)
                arr[i][j]=1;
            else
                arr[i][j]=INT_MAX;
        }
    }

    qDebug()<<"Количество остовных деревьев по теореме Кирхгофа:"<<abs(Kirhgof(arr,matrixVesov));
}
int MainWindow::Kirhgof(QVector<QVector<int>>& arr, QVector<QVector<int>>& matrixVesov)
{

    int count = 0;
    QVector<QVector<int>> buffer(arr.size(), QVector<int>(arr.size()));
    for (int i = 0; i < arr.size(); i++)
    {
        for (int j = 0; j < arr.size(); j++)
            if (arr[i][j] == INT_MAX)
                buffer[i][j] = 0;
            else
            {
                buffer[i][j] = -arr[i][j];
                count++;
            }
        buffer[i][i] = count;
        count = 0;
    }

    int minor = 1;

    QVector<QVector<int>> arr2(arr.size() - 1, QVector<int>(arr.size() - 1));
    for (int i = 1; i < arr.size(); i++)
    {
        for (int j = 1; j < arr.size(); j++)
        {
            arr2[i - 1][j - 1] = buffer[i][j];
        }
    }

    for (int i = 0; i < arr.size() - 1; i++)
        minor += arr2[0][i] * pow(-1, (1 + i + 1)) * findMinor(arr2, 0, i);


    if (arr.size() != 2)
        minor -= 1;
   return abs(minor);


}
int MainWindow::findMinor( QVector<QVector<int>>& buffer, int x, int y)
{
    int minor = 0;

     QVector<QVector<int>> arr(buffer.size() - 1, QVector<int>(buffer.size() - 1));
    int k = 0, f = 0;
    for (int i = 0; i < buffer.size(); i++)
    {
        f = 0;
        for (int j = 0; j < buffer.size(); j++)
        {
            if ((i != x) && (j != y))
            {
                arr[k][f] = buffer[i][j];
                f++;
            }
        }
        if (i != x)
            k++;
    }
    if ((buffer.size() - 1) == 1)
    {
        minor = arr[0][0];
    }
    else
    {
        for (int i = 0; i < buffer.size() - 1; i++)
        {
            minor += arr[0][i] * pow(-1, (1 + i + 1)) * findMinor(arr, 0, i);
        }
    }
    return minor;
}
void MainWindow::FindMaxFlow(){
    int my_begin=1;
    int my_end=general_graph->amount_node;
    if(my_begin<1||my_end<1||my_begin>general_graph->amount_node||my_end>general_graph->amount_node)
        qDebug()<<"Error";
    else
        qDebug()<<"Max flow = "<<general_graph->FindMaxFlow(my_begin,my_end);
}
void MainWindow::AddRandomCosts(){                                                                 // при добавлении стоимостей после добавления ребра программа падает
    matrix_cost=Matrix(this->general_graph->matrix_weight->Mstolb);
    this->matrix_cost=matrix_cost;
    for(int i=1;i<=general_graph->amount_node;i++)
        for(int j=0;j<=general_graph->amount_node;j++)
            if(general_graph->matrix_weight->data[i][j]!=0)
                matrix_cost.data[i][j]=(rand()%10)+1;
    int counter_conection=0;
    for(int i=1;i<=general_graph->amount_node;i++)
        for(int j=0;j<=general_graph->amount_node;j++){
            if(matrix_cost.data[i][j]!=0){
                vector_connections[counter_conection]->AddCost(matrix_cost.data[i][j]);
                counter_conection++;
            }
        }
    int g=0;
    g++;
}
void MainWindow::DeleteAllCosts(){
    matrix_cost=Matrix(this->general_graph->amount_node);
    for(int i=0;i<vector_connections.size();i++){
        vector_connections[i]->DeleteCost();
    }
}
void MainWindow::FindMinCostsMaxFlow(){
    //AddRandomCosts();
    int my_begin=1;
    int my_end=general_graph->amount_node;
    if(my_begin<1||my_end<1||my_begin>general_graph->amount_node||my_end>general_graph->amount_node)
        qDebug()<<"Error";
    else
        qDebug()<<"Min cost 2/3 flow = "<<general_graph->FindFlowMinCost(my_begin,my_end,matrix_cost);
}
//                ///////////////////////////////// 4 лаба/////////////////////////////////////
void MainWindow::ChangeOriented(){
    if(oriented==0){
        for (int i = 1; i < general_graph->matrix_weight->Mstolb+1; ++i)
              for (int j = 1; j < i; ++j)
                  // Присваиваем значениям отраженной пары элементов относительно главной диагонали
                  general_graph->matrix_weight->data[i][j] =  general_graph->matrix_weight->data[j][i];
    }
    else{
        for (int i = 1; i < general_graph->matrix_weight->Mstolb+1; ++i)
              for (int j = 1; j < i; ++j)
                  // Присваиваем значениям отраженной пары элементов относительно главной диагонали
                  general_graph->matrix_weight->data[i][j] = 0;
    }
    LoadTable();
    oriented=!oriented;
}
void MainWindow::FindPrimaKruskal(int flag){
    int iteration_counter=0;
    pair<QVector<pair<int,int>>, int>returning;
    if(flag==0)
        returning=general_graph->FindPrima(iteration_counter);
    if(flag==1)
        returning=general_graph->FindKruskal(iteration_counter);
    qDebug()<<"Минимальный остов: ";
    for(int i=0;i<returning.first.size();i++){
        qDebug()<<returning.first[i].first<<" -> "<<returning.first[i].second;
    }
    qDebug()<<"Длина минимального остова: "<<returning.second;
    qDebug()<<" Количество итераций: "<<iteration_counter;
    matrix_ostov=FromVecPairToMatrix(returning.first);
    SelectPairs(returning.first);
}

void MainWindow::SelectPairs(QVector<pair<int,int>> vec_pair){
    DeselectAll();
    int counter=0;
    QVector<int>vec_connections_for_select;
    for(int i=1;i<general_graph->amount_node+1;i++){
        for(int j=1;j<general_graph->amount_node+1;j++){
           for(int k=0;k<vec_pair.size();k++){
                if(vec_pair[k].first==i&&vec_pair[k].second==j||
                        vec_pair[k].first==j&&vec_pair[k].second==i)
                    vec_connections_for_select.push_back(counter);
            }
           if(general_graph->matrix_weight->data[i][j]!=0)
            counter++;
        }
    }
    for(int i=0;i<vector_nodes.size();i++){
        vector_nodes[i]->Select();
        vector_selected_nodes.push_back(vector_nodes[i]);
    }
    for(int i=0;i<vector_connections.size();i++)
        for(int j=0;j<vec_connections_for_select.size();j++)
            if(i==vec_connections_for_select[j]){
                vector_connections[i]->Select();
                vector_selected_connections.push_back(vector_connections[i]);
            }
}
void MainWindow::CodePrufer(){
   if(matrix_ostov.Mstolb==0){
       qDebug()<<"error";
       return;
   }
   if(matrix_ostov.Mstolb<2){
       qDebug()<<"Количество вершин графа <3. Построение кода Прюфера не имеет смысла. ";
       return;
   }
   qDebug()<<"Матрица остова:";
   matrix_ostov.Print();
   Matrix matrix_ostov_local=matrix_ostov;
   QVector<int> vec_code;
   int amount_node=matrix_ostov_local.Nstring;
   while(vec_code.size()<amount_node-2){
       for(int i=1;i<amount_node+1;i++){
           int amount_one_str=0;
           int j_need;
           for(int j=1;j<amount_node+1;j++){
               amount_one_str+=matrix_ostov_local.data[i][j];
               if(matrix_ostov_local.data[i][j]==1)
                   j_need=j;
           }
           if(amount_one_str==1){
               matrix_ostov_local.data[i][j_need]=0;
               matrix_ostov_local.data[j_need][i]=0;
               vec_code.push_back(j_need);
               break;
           }
       }
   }
   QString str="";
   for(int i=0;i<vec_code.size();i++){
       str+=QString::number(vec_code[i])+=" ";
   }
   qDebug()<<"Код Прюфера: " <<str;
   // ////////////////////////////////// декодируем
   Matrix matrix_decoding(amount_node);
   QVector<int>vec_decoding;
   while(!vec_code.empty()){
       int i_need;
       for(int i=1;i<=amount_node;i++){
           if(std::find(vec_code.begin(),vec_code.end(),i)==vec_code.end()&&
                   std::find(vec_decoding.begin(),vec_decoding.end(),i)==vec_decoding.end()){
                i_need=i;
                break;
           }
       }
       matrix_decoding.data[i_need][*vec_code.begin()]=1;
       matrix_decoding.data[*vec_code.begin()][i_need]=1;
       vec_decoding.push_back(i_need);
       vec_code.erase(vec_code.begin());
   }
   QVector<int> vec_rem;
   for(int i=1;i<=amount_node;i++){//
       if(std::find(vec_decoding.begin(),vec_decoding.end(),i)==vec_decoding.end())
           vec_rem.push_back(i);
   }
   if(vec_rem.size()==2){
       matrix_decoding.data[vec_rem[0]][vec_rem[1]]=1;
       matrix_decoding.data[vec_rem[1]][vec_rem[0]]=1;
   }
   else{
       qDebug()<<"error decoding";
   }
   qDebug()<<"Матрица после декодирования:";
   matrix_decoding.Print();
   bool flag=1;
   for(int i=1;i<=amount_node;i++)
       for(int j=1;j<=amount_node;j++){
           if(matrix_ostov.data[i][j]!=matrix_decoding.data[i][j])
               flag=0;

       }
   if(!flag)
       qDebug()<<"Декодирование не верно";
   else
       qDebug()<<"Декодирование верно";
}

Matrix MainWindow::FromVecPairToMatrix(QVector<pair<int,int>> vec_pair){
    Matrix new_matrix=Matrix(vec_pair.size()+1);
    for(int i=0;i<vec_pair.size();i++){
        new_matrix.data[vec_pair[i].first][vec_pair[i].second]=1;
        new_matrix.data[vec_pair[i].second][vec_pair[i].first]=1;
    }
    return new_matrix;
}
// ///////////////////////////////////// 5 лаба /////////////////////////////////////////////////////////
void MainWindow::FindEulerCircle(){
    bool flag_oriented=1;
    for(int i=1;i<general_graph->amount_node+1;i++)
        for(int j=1;j<general_graph->amount_node+1;j++)
            if(general_graph->matrix_weight->data[i][j]!=general_graph->matrix_weight->data[j][i])
                flag_oriented=0;



    if(!flag_oriented){
    qDebug()<<"Граф неориентированный, поиск невозможен.";
    return;
    }
        Matrix matrix_euler=*general_graph->matrix_weight;
        for(int i=1;i<general_graph->amount_node+1;i++)
            for(int j=1;j<general_graph->amount_node+1;j++)               // сделали из матрицы весов матрицу смежности
                if(matrix_euler.data[i][j]!=0)
                    matrix_euler.data[i][j]=1;
        QVector<int> deg;
        if(isEuler(matrix_euler,deg)){
            qDebug()<<"Граф уже является эйлеровым";
        }
        else{
            matrix_euler=GraphToEuler(matrix_euler);
        }
        *general_graph->matrix_weight=matrix_euler; // поменяли матрицу в графе
        LoadTable();
        SetFromTable();
        if(matrix_euler.Mstolb>2){
            QVector<int> vec_circle=general_graph->GetAilerCircle();
            QVector<pair<int,int>> vec_pair;
            for(int i=0;i<vec_circle.size()-1;i++)
                vec_pair.push_back(make_pair(vec_circle[i],vec_circle[i+1]));
            SelectPairs(vec_pair);
            QString str_out="";
            for(int i=0;i<vec_circle.size();i++){
                if(i!=0)
                    str_out+="->";
                str_out+=QString::number(vec_circle[i]);
            }
             qDebug()<<str_out;
        }
        else
            qDebug()<<"error";
}


bool MainWindow:: isEuler(Matrix matrix_weight,QVector<int>&deg){// deg- от Марго
    bool flag=1;
    deg.clear();
    for(int i=1;i<matrix_weight.Mstolb+1;i++){
        int sum=0;
        for(int j=1;j<matrix_weight.Mstolb+1;j++){
            if(matrix_weight.data[i][j]!=0){
                sum++;
            }
        }
        if(sum%2==1){
            flag=0;
            deg.push_back(i-1);
        }
        if(sum==0){
            deg.clear();
             for(int j=1;j<matrix_weight.Mstolb+1;j++){
                 deg.push_back(j-1);
                 return false;
             }
        }
    }
    return flag;
}



Matrix MainWindow::GraphToEuler( Matrix matrix_weight/*QVector<QVector<int>>& EulerGr*/) {
    QVector<int> deg;

    int versh =matrix_weight.Nstring;
    int n = versh;
    if (versh == 2) {
        qDebug() << "\nВ графе 2 вершины! Эйлеров цикл не может существовать!" << endl << endl;

        return matrix_weight;
    }
    int  eu=isEuler(matrix_weight,deg);
    bool flag = 0;
    while (!eu) {//еслии нет
        flag = 1;
        QVector<int> degree1;//вектор степеней вершин
        for (int i = 0; i < n; i++) {
            int sst = 0;// вот тут ломается
            for (int j = 0; j < n; j++) {
                if ( matrix_weight.data[i+1][j+1]/*EulerGr[i][j]*/ != 0) sst++;
            }
            degree1.push_back(sst);
        }
        srand(time(NULL));
        bool isChanged = false;
        for (int i = 0; i < deg.size(); i++) {//проходим по всем вариантам, которые сохраняли в функции-проверке
            for (int j = i + 1; j < deg.size(); j++) {//причем только верхний треугольник, тк граф неориентированный
                if ( matrix_weight.data[deg[i]+1][deg[j]+1]== 0) {//если нет ребра
                     matrix_weight.data[deg[i]+1][deg[j]+1] = 1;//добавляе
                     matrix_weight.data[deg[j]+1][deg[i]+1] = 1;
                    qDebug() << "Добавляем ребро " << deg[i] + 1 << "-" << deg[j] + 1 << endl;
                    isChanged = true;//отметили изменение
                    break;
                }

                if ( matrix_weight.data[deg[i]+1][deg[j]+1]/*EulerGr[deg[i]][deg[j]] */== 1) {//если ребро есть
                     matrix_weight.data[deg[i]+1][deg[j]+1]/*EulerGr[deg[i]][deg[j]]*/ = 0;//удалаем его, обновляем значения
                     matrix_weight.data[deg[j]+1][deg[i]+1]/*EulerGr[deg[j]][deg[i]]*/ = 0;
                    qDebug() << "Удаляем ребро " << deg[i] + 1 << "-" << deg[j] + 1 << endl;

                    if (degree1[deg[i]] == 1 && degree1[deg[j]] % 2 != 0 && degree1[deg[j]] != 1)//степень одной вершины равна 1, степень второй нечетная и не равна 1
                    {
                        if (deg[i] != n - 1)//добавляем ребра
                        {
                             matrix_weight.data[deg[i]+1][n-1+1]/*EulerGr[deg[i]][n - 1]*/ = 1;
                             matrix_weight.data[n-1+1][deg[i]+1]/*EulerGr[n - 1][deg[i]]*/ = 1;
                            qDebug() << "Добавляем ребро: " << deg[i] + 1 << "-" << n << endl;// убрать +1
                        }
                        else
                        {
                             matrix_weight.data[deg[i]+1][1]/*EulerGr[deg[i]][0]*/ = 1;
                             matrix_weight.data[1][deg[i]+1]/*EulerGr[0][deg[i]]*/ = 1;
                            qDebug() << "Добавляем ребро: " << 1 << "-" << deg[i] + 1 << endl;
                        }
                        if (deg[i] != n - 2)
                        {
                             matrix_weight.data[deg[i]+1][n-2+1]/*EulerGr[deg[i]][n - 2]*/ = 1;
                             matrix_weight.data[n-2+1][deg[i]+1]/*EulerGr[n - 2][deg[i]]*/ = 1;
                            qDebug() << "Добавляем ребро: " << deg[i] + 1 << "-" << n - 1 << endl;
                        }
                        else
                        {
                             matrix_weight.data[i+1][1]/*EulerGr[deg[i]][0]*/ = 1;
                             matrix_weight.data[1][i+1]/*EulerGr[0][deg[i]]*/ = 1;
                            qDebug() << "Добавляем ребро: " << 1 << "-" << deg[i] + 1 << endl;
                        }

                    }

                    if (degree1[deg[j]] == 1 && degree1[deg[i]] % 2 != 0 && degree1[deg[i]] != 1)//аналогично предыдущим действиям, только в обратную сторону
                    {
                        if (deg[j] != n - 1 &&  matrix_weight.data[deg[i]+1][deg[j]+1]/*EulerGr[deg[i]][deg[j]]*/ == 0)
                        {
                             matrix_weight.data[deg[j]+1][n-1+1]/*EulerGr[deg[j]][n - 1]*/ = 1;
                             matrix_weight.data[n-1+1][deg[i]+1]/*EulerGr[n - 1][deg[i]]*/ = 1;
                            qDebug() << "Добавляем ребро: " << deg[j] + 1 << "-" << n << endl;
                        }
                        else
                        {
                             matrix_weight.data[deg[j]+1][1]/*EulerGr[deg[j]][0]*/ = 1;
                             matrix_weight.data[1][deg[j]+1]/*EulerGr[0][deg[j]] */= 1;
                            qDebug() << "Добавляем ребро: " << 1 << "-" << deg[j] + 1 << endl;
                        }
                        if (deg[j] != n - 2 &&  matrix_weight.data[deg[i]+1][deg[j]+1]/*EulerGr[deg[i]][deg[j]] */== 0)
                        {
                             matrix_weight.data[deg[j]+1][n-2+1]/*EulerGr[deg[j]][n - 2]*/ = 1;
                             matrix_weight.data[n-2+1][deg[j]+1]/*EulerGr[n - 2][deg[j]] */= 1;
                            qDebug() << "Добавляем ребро: " << deg[j] + 1 << "-" << n - 1 << endl;
                        }
                        else
                        {
                             matrix_weight.data[deg[j]+1][1]/*EulerGr[deg[j]][0] */= 1;
                             matrix_weight.data[1][deg[j]+1]/*EulerGr[0][deg[j]]*/ = 1;
                            qDebug() << "Добавляем ребро: " << 1 << "-" << deg[j] + 1 << endl;
                        }

                    }
                    isChanged = true;//отмечаем изменения
                    break;

                }
            }
            if (isChanged)

                break;
        }

        deg.clear();//очищаем вектор возможных вершин

//            eu = Euler(EulerGr);//проверяем
          eu=isEuler(matrix_weight,deg);
    }

    if (flag) {//если меняли граф

        qDebug() << "Измененная матрица: " << endl;

//            outArr(EulerGr);
        matrix_weight.Print();
    }
    qDebug() << endl;

    deg.clear();
    return matrix_weight;
}

void MainWindow::BuildGamilton(){
    srand(0);
    bool flag_oriented=1;
    for(int i=1;i<general_graph->amount_node+1;i++)
        for(int j=1;j<general_graph->amount_node+1;j++)
            if(general_graph->matrix_weight->data[i][j]!=general_graph->matrix_weight->data[j][i])
                flag_oriented=0;
    if(!flag_oriented){
    qDebug()<<"Граф неориентированный, алгоритм работает только для ориентированного графа.";
    return;
    }
    bool flag_g=1;
    QVector<int> bad_vertix;
    for(int i=1;i<=general_graph->amount_node;i++){
       if(!VertixIsAbleForGamilton(general_graph->matrix_weight->data[i]))
          bad_vertix.push_back(i);
    }
    if(general_graph->amount_node<3){
        qDebug()<<"Слишком мало вершин";
        return;
    }
    if(bad_vertix.empty()){
        qDebug()<<"Заданный граф - Гамильтонов.";
    }
    else{
        qDebug()<<"Заданный граф не является Гамильтоновым";
        for(int i=0;i<bad_vertix.size();i++){
            int counter_j=i;
            while(!VertixIsAbleForGamilton(general_graph->matrix_weight->data[bad_vertix[i]])){
                if(bad_vertix[i]!=counter_j){

                general_graph->matrix_weight->data[bad_vertix[i]][counter_j]=1;
                general_graph->matrix_weight->data[counter_j][bad_vertix[i]]=1;
                }
                counter_j++;
            }
        }
        qDebug()<<"Гамильтонов граф:";
        general_graph->matrix_weight->Print();
        LoadTable();
        SetFromTable();
    }
}
bool MainWindow::VertixIsAbleForGamilton(QVector<int> v){
    int sum=0;
    for(int i=1;i<v.size();i++){
        if(v[i]!=0)
            sum++;
    }
    if(sum<(v.size())/2)
        return false;
    else
        return true;
}
void MainWindow::FindComivoajor(){
    BuildGamilton();
    QVector<pair<QVector<int>,int>> returning=general_graph->GetAllGamiltonCircle();
    int min_way=INT_MAX;
    int min_i=0;
    qDebug()<<"Все пути:";
    for(int i=0;i<returning.size();i++){
        if(returning[i].second<min_way){
            min_way=returning[i].second;
            min_i=i;
        }
        PrintVecWay(returning[i].first);
        qDebug()<<"Длина пути: "<<returning[i].second;
    }

    //PrintVecWay(QVector<int>{1,2,3});
    qDebug()<<"MIn way: "<<returning[min_i].second;
    PrintVecWay(returning[min_i].first);
    QVector<int>vec_min_way=returning[min_i].first;
    vec_min_way.push_back(1);
    QVector<pair<int,int>>vec_pair;
    for(int i=0;i<vec_min_way.size()-1;i++){
        vec_pair.push_back(make_pair(vec_min_way[i],vec_min_way[i+1]));
    }
    SelectPairs(vec_pair);
}

void MainWindow::PrintVecWay(QVector<int>vec){
    QString str="";
    for(int i=0;i<vec.size();i++){
        if(i!=0)
            str+="->";
        str+=QString::number(vec[i]);
    }
    qDebug()<<str;
}












