#include "graph.h"

Graph::Graph(){
   // QVector<int> h={0};
    matrix_weight=new Matrix();
}
Graph::Graph(QString str)
{
 QString str1="";

 while(str.size()!=0){
 str1=str.mid(0,str.indexOf(':')-1>0?str.indexOf(':')-1:0);
 str.remove(0,str.indexOf(':')-1>0?str.indexOf(':'):0);
 int i=0;
 int number=str1.toInt();
 while(str[i]!='\n'){
     str1="";
     while(str[i]!=' '&&str[i]!='\n'){
         str1+=str[i];
         i++;
     }
     if(str[i]!='\n')
         i++;
     this->list_adjacency[number].push_back(str1.toInt());
 }
 }
}

Graph::Graph(QStandardItemModel* model){
    if(model->columnCount()==model->rowCount()){
        if(matrix_weight!=nullptr)
        {
            qDebug()<<"Deleted matrix:";
            matrix_weight->Print();
            delete matrix_weight;
        }

        matrix_weight = new Matrix(model);
     //   qDebug()<<"MAtrix:";
      //  matrix_weight->Print();
        amount_node = model->columnCount(); // количество строк в модели

        Matrix mul=(*matrix_weight)*(*matrix_weight);
       // qDebug()<<"after *";
      //  mul.Print();
      //  qDebug()<<"after MultiplyForShimbel";
        mul=matrix_weight->MultiplyForShimbel(*matrix_weight);
      //  mul.Print();
    }

    else{
        qDebug()<<"error table";
        qDebug()<<model->rowCount();
        qDebug()<<model->columnCount();
    }

}
Graph::Graph(int amount_node){
    QVector<int> vec_distribution=GetGeometricDistribution(amount_node);
    QVector<int> h_vec;
    for(int i=0;i<amount_node+1;i++){
        h_vec.push_back(0);
    }
    QVector<QVector<int>>for_matrix;
    for_matrix.push_back(h_vec);
    for(int i=1;i<amount_node;i++){
        int amount_connect=GetRandomValueFromVecDistribution(vec_distribution);
        int amount_place=amount_node-i;
        QVector<int>help_vec;
        for(int k=0;k<=i;k++){
            help_vec.push_back(0);
        }
        help_vec.append(GenerateRandomVector(amount_connect,amount_place));
        for_matrix.push_back(help_vec);
    }
    for_matrix.push_back(h_vec);
    this->matrix_weight=new Matrix(for_matrix);
}
void Graph::AddNode(){
    if(matrix_weight==NULL)
        matrix_weight=new Matrix();
    for(int i=0;i<amount_node+1;i++){
        matrix_weight->data[i].push_back(0);
    }
    QVector<int> help_vec;
    for(int i=0;i<amount_node+2;i++){
        help_vec.push_back(0);
    }
    matrix_weight->data.push_back(help_vec);
    amount_node++;
    matrix_weight->Mstolb++;
    matrix_weight->Nstring++;
}
void Graph::DeleteNode(int number){
    if(number<1){
        qDebug()<<"Error removing";
        return;
    }
    for(int i=0;i<amount_node+1;i++){
        matrix_weight->data[i].erase(matrix_weight->data[i].begin()+number);
   }
    matrix_weight->data.erase(matrix_weight->data.begin()+number);
    matrix_weight->Mstolb--;
    matrix_weight->Nstring--;
    amount_node--;

}
void Graph::AddConnection(int number1,int number2,int weight){
    matrix_weight->data[number1][number2]=weight;
  //  matrix_graph[number2][number1]=1;
}

Graph::~Graph(){
    delete matrix_weight;
}
void Graph::CreateListAdjacency(){
    QVector<int> help_vec;
    list_adjacency.push_back(help_vec);
    for(int i=1;i<=matrix_weight->Nstring;i++){
         QVector<int> help_vec;
         for(int j=1;j<=matrix_weight->Nstring;j++){
             if(matrix_weight->data[i][j]!=0)
                 help_vec.push_back(j);
         }
         list_adjacency.push_back(help_vec);
    }
}
void Graph::PrintListAdjacency(){
    QString str="";
    int  counter=1;
    for_each(list_adjacency.begin()+1,list_adjacency.end(),[&counter](QVector<int> a){
        QString str=QString::number(counter)+": ";
        for_each(a.begin(),a.end(),[&str](int a){
           str+=QString::number(a)+=" ";
        });
                qDebug()<<str;
                counter++;
    });
}
bool Graph::FindWaysBFS(int node_begin,int node_end,int& amount,QVector<QVector<int>>& ways){
    if(amount_node==0||node_begin>amount_node||node_end>amount_node){
        qDebug()<<"Error search";
        amount=0;
        ways.clear();
        return 0;
    }
   QVector<int> visited;
   for(int i=0;i<=amount_node;i++){
       visited.push_back(numeric_limits<int>::max());
   }
   QQueue<int> queue_nodes;
   QVector<QVector<int>> vector_ways;
   QVector<int>best_way;
   for(int i=0;i<=amount_node;i++){
       QVector<int> help;
       vector_ways.push_back(help);
   }
   queue_nodes.push_back(node_begin);
   visited[node_begin]=0;
   int counter=0;
   while(!queue_nodes.empty()){
       for(int j=1;j<=amount_node;j++){
           if(matrix_weight->data[queue_nodes.front()][j]!=0){ // вершины связаны
               if(counter<visited[j]){
                   visited[j]=counter;
                   best_way.push_back(queue_nodes.front());
                queue_nodes.push_back(j);
               }
                vector_ways[j].push_back(queue_nodes.front());//queue_nodes.front());
           }
       }
   queue_nodes.pop_front();
   counter++;
   }
   bool flag;
   if(visited[node_end]!=numeric_limits<int>::max())
       flag=1;
   else
       flag=0;
   pair<int,QVector<QVector<int>>>help_pair;
   help_pair=FindAmountWays(vector_ways, node_end,node_begin);//пока
   amount=help_pair.first;
   ways=help_pair.second;
   qDebug()<<"Max ways:";
   PrintMaxWaysShimbel(node_begin,node_end);
   qDebug()<<"Min ways:";
   PrintMinWaysShimbel(node_begin,node_end);
   return flag;
}
pair<int,QVector<QVector<int>>>Graph::FindAmountWays(const QVector<QVector<int>>& list_ways,int current_node,int final_node){
    // при больших размерах графа, фуекция работает очень медленно и расходует много памяти
    int sum=0;
    QVector<QVector<int>>help_table;
    if(current_node==final_node)
        return pair<int,QVector<QVector<int>>>(1,{{final_node}});
    else
        for_each(list_ways[current_node].begin(),list_ways[current_node].end(),
                 [this,&sum,list_ways,&final_node,&help_table](int a){
            pair<int,QVector<QVector<int>>>help_pair;
            help_pair=FindAmountWays(list_ways,a,final_node);
            sum+=help_pair.first;
            help_table.append(help_pair.second);
        });
    for_each(help_table.begin(),help_table.end(),[&current_node](QVector<int>& a){
        a.push_back(current_node);
    });
    return pair<int,QVector<QVector<int>>>(sum,help_table);
}
QVector<int> Graph::GetGeometricDistribution(int amount){
     double property_succes;//=0.2;//0.5 - 11(0), 0.4 - 21(0)                  //вот тут  вопрос с распределением
     if(amount>=10)
         property_succes=0.1;
     if(amount>6&&amount<10)
         property_succes=0.2;
     if(amount>4&&amount<=6)
         property_succes=0.4;
     if(amount>2&&amount<=4)
          property_succes=0.6;
     if(amount<=2)
          property_succes=0.7;
     double mul=1;
     QVector<double>vec_property;
     mul=property_succes;
     vec_property.push_back(0);
     vec_property.push_back(mul);
     for(int i=1;i<amount-1;i++){
         mul=mul*(1-property_succes);
         vec_property.push_back(mul);
     }
     double sum=0;
     for(int i=1;i<vec_property.size();i++ )
         sum+=vec_property[i];
     vec_property[0]=1 - sum;
     QVector<int>returned;
     for(int i=0;i<vec_property.size();i++){
         for(int j=0;j<vec_property[i]*100;j++){
             returned.push_back(i);
         }
     }
     return returned;
}
int Graph::GetRandomValueFromVecDistribution(QVector<int> vec_distribution){
  //  srand(time(0));
    int random_number=rand()%vec_distribution.size();
    return vec_distribution[random_number];
}
QVector<int> Graph::GenerateRandomVector(int n, int my_size) {
    static int counter=0;
    counter++;
   QVector<int> v(my_size, 0);
    if (n >= my_size) {

          for (int i = 0; i < my_size; i++) {
               v[i]=(std::rand()%9+1);
          }
        return v;
    }


    for (int i = 0; i < n; i++) {
        int idx = std::rand() % my_size;
        while (v[idx] != 0) {
            idx = std::rand() % my_size;
        }
        v[idx] = std::rand()%9+1;
    }

    return v;
}
int Graph::PrintMaxWaysShimbel(int current_node,int final_node){
    Matrix res_matrix;
    res_matrix=*matrix_weight;
    QVector<int>vec_res;
    for(int i=0;i<amount_node;i++){
        int res =res_matrix.data[current_node][final_node];
        vec_res.push_back(res);
        if(res!=0)
        qDebug()<<"Max path through "<<i+1<<" edge = "<<res;// /////////////////////////////////
        res_matrix=res_matrix.MultiplyForShimbel(*matrix_weight,1);
    }
    int max=*std::max_element(vec_res.begin(),vec_res.end());
    if(max!=0)
        qDebug()<<"Common max path = "<<max;
    else
        qDebug()<<"No ways";
    return max;
}
int Graph::PrintMinWaysShimbel(int current_node,int final_node){
    Matrix res_matrix;
    res_matrix=*matrix_weight;
    QVector<int>vec_res;
    for(int i=0;i<amount_node;i++){
        int res =res_matrix.data[current_node][final_node];
        vec_res.push_back(res);
        if(res!=0)
        qDebug()<<"Min path through "<<i+1<<" edge = "<<res;
        res_matrix=res_matrix.MultiplyForShimbel(*matrix_weight,0);
    }
    int min;
    vec_res.removeAll(0);
    if(!vec_res.empty()){
    min=*std::min_element(vec_res.begin(),vec_res.end());
    qDebug()<<"Common min path = "<<min;
    }
    else{
       qDebug()<<"No path";
    }
    return min;
}
//                                                /////////////////////////////////////////////////////////// 2 лаба
pair<QVector<int>,int>  Graph::FindWayDijkstra(int node_begin,int node_end,int& iteration_counter){
    iteration_counter=0;
    QVector<int> distance;// заполняем с нуля
    QVector<int> visited;
    QVector<int> where_from;
    for(int i=0;i<amount_node;i++){
        distance.push_back(INT_MAX);
        visited.push_back(0);
        where_from.push_back(-1);
    }
    distance[node_begin-1]=0;
    int current_node=node_begin-1;
    int distance_current_node=0;
    bool flag =1;
    while(flag){
        for(int i=0;i<amount_node;i++){
            if(distance_current_node!=INT_MAX&&matrix_weight->data[current_node+1][i+1]!=0)
                if(distance_current_node+matrix_weight->data[current_node+1][i+1]<distance[i]){
                     distance[i]=distance_current_node+matrix_weight->data[current_node+1][i+1];
                     where_from[i]=current_node+1;
                }
            iteration_counter++;
        }visited[current_node]=1;
        int min_distance_node=INT_MAX;
        int i_min;
        flag=0;
        for(int i=0;i<amount_node;i++){
            if(visited[i]==0 && distance[i]<=min_distance_node){
                min_distance_node=distance[i];
                i_min=i;
                flag=1;
            }
             iteration_counter++;
        }
        current_node=i_min;
        distance_current_node=min_distance_node;
    }
    int help_node=node_end;
    QVector<int>returned;
    if(distance[node_end-1]!=INT_MAX){
        while(help_node!=-1){
            returned.push_back(help_node);
            help_node=where_from[help_node-1];
        }
        QVector<int>rev_ret;
        while(!returned.empty()){
            rev_ret.push_back(returned.back());
            returned.pop_back();
        }
        return  pair<QVector<int>,int> (rev_ret,distance[node_end-1]);
    }
    else
        return  pair<QVector<int>,int> ({-1},INT_MAX);

}
pair<QVector<int>,int> Graph::FindBelmanFord(int node_begin,int node_end,int& iteration_counter){
    iteration_counter=0;
    if(node_begin>amount_node||node_end>amount_node){
        qDebug()<<"Error Belman-Ford";
        return pair<QVector<int>,int>({},0);
    }
    QVector<int>my_table;
    for(int i=0;i<amount_node;i++){
        my_table.push_back(INT_MAX);
    }
   QVector<QVector<int>>vec_ways(amount_node, QVector<int>{});
    my_table[node_begin-1] = 0;
    vec_ways[node_begin-1].push_back(node_begin);
    for(int k=1;k<amount_node;k++){
        for (int i1 = 0; i1< amount_node; i1++){
            for(int j=0;j<matrix_weight->Mstolb;j++){
                iteration_counter++;
                if (my_table[i1] + matrix_weight->data[i1+1][j+1] <= my_table[j]
                        &&my_table[i1]!=INT_MAX&&matrix_weight->data[i1+1][j+1] !=0){
                    my_table[j] = my_table[i1] + matrix_weight->data[i1+1][j+1];
                    vec_ways[j]=vec_ways[i1];
                    vec_ways[j].push_back(j+1);
                }
          }
       }
    }
    return pair<QVector<int>,int>(vec_ways[node_end-1],my_table[node_end-1]);
}
pair<QVector<int>,int> Graph::FindFloydWarshal(int node_begin, int node_end, int &iteration_counter){
    iteration_counter=0;
    if(node_begin>amount_node||node_end>amount_node){
        qDebug()<<"Error Belman-Ford";
        return pair<QVector<int>,int>({},0);
    }
   QVector<QVector<int>>my_matrix;
   my_matrix=matrix_weight->data;
   QVector<int>vec_way_from;
       vec_way_from.push_back(-1);
   for(int k=1;k<=amount_node;k++){
       vec_way_from.push_back(-1);
       for(int i=1;i<=amount_node;i++){
           if(my_matrix[k][i]==0)
               my_matrix[k][i]=INT_MAX;
       }
   }
    for(int i=1;i<=amount_node;i++){
        if(matrix_weight->data[node_begin][i]!=0)
            vec_way_from[i]=node_begin;
    }
   for(int k=1;k<=amount_node;k++){
       for(int i=1;i<=amount_node;i++){
           for(int j=1;j<=amount_node;j++){
               iteration_counter++;
               if(my_matrix[i][k]!=INT_MAX&&my_matrix[k][j]!=INT_MAX&&my_matrix[i][k]+my_matrix[k][j]<my_matrix[i][j]){
                   my_matrix[i][j]=my_matrix[i][k]+my_matrix[k][j];
                   if(i==node_begin)
                   vec_way_from[j]=k;
               }
           }
       }
   }
   QVector<int>returned;
   int help_node;
   returned.push_back(node_end);
   int begin_point=node_end;
   while(begin_point!=node_begin){
           if(vec_way_from[begin_point]!=-1)
               help_node=vec_way_from[begin_point];
           else{
               vec_way_from.clear();
               break;
           }
       begin_point=help_node;
       returned.push_back(begin_point);
   }
   QVector<int>rev_ret;
   while(!returned.empty()){
       rev_ret.push_back(returned.back());
       returned.pop_back();
   }
    return pair<QVector<int>,int>(rev_ret,my_matrix[node_begin][node_end]);
}



