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
Graph::Graph(Matrix m){
   // Matrix m1=m;
    this->matrix_weight=new Matrix(m.data);
   /* this->matrix_weight->data=m.data;
    this->matrix_weight->Mstolb=m.Mstolb;
    this->matrix_weight->Nstring=m.Nstring;*/
    amount_node=m.Mstolb;
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
    // ///                  для задания единственного стока в графе
    for(int j=2;j<=amount_node;j++){
        int sum=0;
        for(int i=0;i<=amount_node;i++){
            sum+=matrix_weight->data[i][j];
        }
        if(sum==0)
            matrix_weight->data[1][j]=1;
    }
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
     for(int i=0;i<returned.size();i++){
         if(returned[i]==0)
             returned[i]=1;
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
// ////////////////////////////////////  3 лаба////////////////////////////////////////////
int Graph::FindMaxFlow(int v_begin, int v_end){
    Matrix  matrix_flow=*(this->matrix_weight);
    Matrix  matrix_reverse_flow(this->amount_node); // создали матрицу заполненную нулями
    QVector<pair<int,int>>vec_label(this->amount_node+1,pair<int,int>(-1,-1));                              // величина/откуда пришли
    int general_sum=0;

    int sum_bound_counter=0;
    for(int i=1;i<matrix_flow.data.size();i++)
        sum_bound_counter+=matrix_flow.data[v_begin][i];
    while(sum_bound_counter>0){
    int i=v_begin;
    vec_label[v_begin].first=INT_MAX;
    vec_label[v_begin].second=0;
    while(vec_label[v_end].second==-1){
    map<int,int> M_i;
    for(int j=1;j<matrix_flow.data.size();j++){
        if(matrix_flow.data[i][j]>0&&vec_label[j].second==-1)
            M_i.insert(pair<int,int>(j,matrix_flow.data[i][j]));
    }
    if(!M_i.empty()){
        map<int,int>::iterator it=M_i.begin();
        pair<int,int>k_max=pair<int,int>(0,0);
        for(;it!=M_i.end();it++){
            if(it->second>k_max.second){
                k_max=*it;
            }
        }
        vec_label[k_max.first].first=k_max.second;
        vec_label[k_max.first].second=i;
        i=k_max.first;
    }
    else{
        if(i==1)
            break;
        int prev_node=vec_label[i].second;
        for(int j=1;j<matrix_flow.data.size();j++){
            if(matrix_flow.data[prev_node][j]>0&&vec_label[j].second==-1&&j!=i)
                M_i.insert(pair<int,int>(j,matrix_flow.data[prev_node][j]));
        }i=prev_node;

    }

}
    int min_value=INT_MAX;
    int current_vertex_from=vec_label[v_end].second;
    int current_vertex_flow=vec_label[v_end].first;
    if(current_vertex_from==-1)
        break;
    while(current_vertex_from!=v_begin){                                                   // шаг 5. Находим минимальное значение пропускаемости
        if(min_value>current_vertex_flow){
            min_value=current_vertex_flow;
        }
        current_vertex_flow=vec_label[current_vertex_from].first;
        current_vertex_from=vec_label[current_vertex_from].second;// тут явно что-то не так с индексами
    }
    if(min_value>current_vertex_flow){
        min_value=current_vertex_flow;
    }
    current_vertex_from=vec_label[v_end].second;
    matrix_flow.data[current_vertex_from][v_end]-=min_value;
    matrix_reverse_flow.data[current_vertex_from][v_end]+=min_value;
    while(current_vertex_from!=v_begin){                                                   // уменьшим поток и увеличим обратный поток
        matrix_flow.data[vec_label[current_vertex_from].second][current_vertex_from]-=min_value;
        matrix_reverse_flow.data[vec_label[current_vertex_from].second][current_vertex_from]+=min_value;
        current_vertex_from=vec_label[current_vertex_from].second;
    }

    general_sum+=min_value;
    for(int k=0;k<vec_label.size();k++)
        vec_label[k]=pair<int,int>(-1,-1);
    sum_bound_counter=0;
    for(int i=1;i<matrix_flow.data.size();i++)
        sum_bound_counter+=matrix_flow.data[v_begin][i];
}

return general_sum;
}

int Graph::FindFlowMinCost(int v_begin, int v_end,Matrix matrix_cost){
    int general_flow=(2*FindMaxFlow( v_begin, v_end))/3;

    Graph help_graph(matrix_cost);
    int waste_value;
    pair<QVector<int>,int> pair_min_cost;
    Matrix help_matrix_flow(matrix_weight->data);

    int min_cost_returned=0;
    while (general_flow>0) {
        pair_min_cost=help_graph.FindBelmanFord(v_begin,v_end,waste_value);
        int min_cost=pair_min_cost.second;
        int local_max_flow=FindMaxFlowOneWay(pair_min_cost.first,help_matrix_flow);
        QString str="";
        for(int i=0;i<pair_min_cost.first.size();i++){
            if(i!=0)
                str+="->";
            str+=QString::number(pair_min_cost.first[i]);
        }
        for(int i=1;i<amount_node+1;i++){
            for(int j=1;j<amount_node+1;j++){
               if(help_matrix_flow.data[i][j]==0)
                   help_graph.matrix_weight->data[i][j]=0;  //записали нули в места, где поток заполнен
            }
        }
        //qDebug()<<"матрица стоимости после изменеия: ";
      //  help_graph.matrix_weight->Print();
        if(local_max_flow<general_flow){
            min_cost_returned+=min_cost*local_max_flow;

        }
        else
            min_cost_returned+=min_cost*general_flow;
        general_flow-=local_max_flow;
        qDebug()<<"Количество потока "<<local_max_flow<<"проходит по пути"<<str;
    }
    return min_cost_returned;

}
int Graph::FindMaxFlowOneWay(QVector<int> vec_way,Matrix my_matrix){
    int min_value=INT_MAX;
    for(int i=0;i<vec_way.size()-1;i++){
        if(my_matrix.data[vec_way[i]][vec_way[i+1]]<min_value)
            min_value=my_matrix.data[vec_way[i]][vec_way[i+1]];
    }
    return min_value;
}
void Graph::ReduceWayInMatrix(QVector<int> vec_way, int value_reduce, Matrix &my_matrix){
    for(int i=0;i<vec_way.size()-1;i++){
        my_matrix.data[vec_way[i]][vec_way[i+1]]-=value_reduce;
    }
}
// ////////////////////////////////////////////4 лаба/////////////////////////
pair<QVector<pair<int,int>>,int>Graph::FindPrima(int&iteration_counter){
    iteration_counter=0;
    QVector<int> selected_v;
    QVector<int> currents_v;
    QVector<pair<int,int>> vec_pair_edges;
    currents_v.push_back(1);
    int min_value=INT_MAX;
    int i_min;
    int j_min;
    int sum=0;
    while(currents_v.size()<amount_node){
        int min_value=INT_MAX;
        for(int i=0;i<currents_v.size();i++){
            for(int j=1;j<=matrix_weight->Mstolb;j++){
                if(matrix_weight->data[currents_v[i]][j]!=0&&
                        matrix_weight->data[currents_v[i]][j]<min_value&&
                        find(currents_v.begin(), currents_v.end(), j)==currents_v.end()){
                    min_value=matrix_weight->data[currents_v[i]][j];
                    i_min=currents_v[i];
                    j_min=j;
                }
                iteration_counter++;
            }
        }
        vec_pair_edges.push_back(make_pair(i_min,j_min));
        selected_v.push_back(j_min);
        currents_v.push_back(j_min);
        sum+=min_value;
    }
    return make_pair(vec_pair_edges,sum);
}



pair<QVector<pair<int,int>>,int>Graph::FindKruskal(int&iteration_counter){
    QVector<pair<int,int>>returned;
    iteration_counter=0;
    multimap<int,pair<int,int>> map_edge;
    vector<vector<int>> vec_comp_link;
    for(int i=1;i<amount_node+1;i++){
        vector<int> help_vec{i};
        vec_comp_link.push_back(help_vec);
    }
    for (int i = 1; i <=amount_node; i++){
          for (int j = i; j <=amount_node; j++){
              if(matrix_weight->data[i][j]!=0){
                  map_edge.insert( make_pair(matrix_weight->data[i][j],make_pair(i,j)));
              }
          }
    }
    int sum_ostov=0;
    while(vec_comp_link.size()>1){

        int lenght=map_edge.begin()->first;
        int v1=map_edge.begin()->second.first;
        int v2=map_edge.begin()->second.second;
        for(int i=0;i<vec_comp_link.size();i++){

            if(find(vec_comp_link[i].begin(),vec_comp_link[i].end(),v1)!=vec_comp_link[i].end())   {                 // если в текущей комп связности есть вершина 1
                 if(find(vec_comp_link[i].begin(),vec_comp_link[i].end(),v2)!=vec_comp_link[i].end()){               // если вершина 2 в ней же
                     map_edge.erase(map_edge.begin());
                     iteration_counter++;
                     break;
                 }
                 else{              //если вершина 2 в другой компоненте
                     sum_ostov+=lenght;
                     returned.push_back(map_edge.begin()->second);
                     map_edge.erase(map_edge.begin());
                     for(int j=0;j<vec_comp_link.size();j++){
                         iteration_counter++;
                         if(find(vec_comp_link[j].begin(),vec_comp_link[j].end(),v2)!=vec_comp_link[j].end()){      // поместили вершины из второй компоненты в первую
                             vec_comp_link[i].insert(vec_comp_link[i].end(), vec_comp_link[j].begin(), vec_comp_link[j].end());
                             vec_comp_link.erase(vec_comp_link.begin()+j);
                             break;
                         }
                     }
                     break;
                 }
            }
            else{
                iteration_counter++;
            }
        }
    }
    return make_pair(returned,sum_ostov);
}
QVector<int> Graph::GetAilerCircle(){
    QVector<int> vec_visited_v;
    QVector<int> vec_circle_v;
    int current_v=1;
    int begin_v;
    vec_visited_v.push_back(1);
    bool flag_v_here=1;
    Matrix help_matrix_weight=*matrix_weight;
    int amount_edge=0;
    for(int i=1;i<amount_node+1;i++)
        for(int j=1;j<amount_node+1;j++)
            if(help_matrix_weight.data[i][j]!=0)
                amount_edge++;
    amount_edge/=2;
    while(amount_edge>0){
        flag_v_here=1;
        while(flag_v_here){                       //может быть надо поменять условие
            flag_v_here=0;
            for(int i=1;i<amount_node+1;i++){
                if(help_matrix_weight.data[current_v][i]!=0){
                    vec_visited_v.push_back(i);
                    help_matrix_weight.data[current_v][i]=0;
                    help_matrix_weight.data[i][current_v]=0;
                    amount_edge--;
                    flag_v_here=1;
                    current_v=i;
                    break;
                }
            }

        }

        while(!flag_v_here&&!vec_visited_v.empty()){
            for(int i=0;i<amount_node+1;i++)
                if(help_matrix_weight.data[current_v][i]!=0){
                    flag_v_here=1;
                   // current_v=i;
                    break;
                }
            if(!flag_v_here){
                if(vec_visited_v.empty())
                    qDebug()<<"error";
                else{
                    vec_circle_v.push_back(vec_visited_v.back());

                    vec_visited_v.pop_back();
                    if(!vec_visited_v.empty())
                        current_v=vec_visited_v.back();
                    else
                        break;
                }  
            }
            else{
               //vec_visited_v.push_back(current_v);

            }
        }
        int g=0;
        g++;
    }
    return vec_circle_v;
    }
QVector<pair<QVector<int>,int>> Graph::GetAllGamiltonCircle(){
    QVector<pair<QVector<int>,int>> returned;
     int begin_node=1;
     QQueue<int> Q;
     Q.push_back(begin_node);
     Tree tree_graph(begin_node);
     while(!Q.empty()){
         int current_node=Q.front();
         Q.pop_front();
         for(int i=1;i<amount_node+1;i++){
             if(matrix_weight->data[current_node][i]!=0)
                 if(tree_graph.AddNode(i,current_node)==true)
                     Q.push_back(i);
         }
     }
     for(int i=0;i<tree_graph.vec_node_node.size();i++){
         if(tree_graph.vec_node_node[i].vec_parents.size()==amount_node-1 &&
                  matrix_weight->data[tree_graph.vec_node_node[i].number][begin_node]!=0){
             returned.push_back(make_pair(tree_graph.vec_node_node[i].vec_parents,0));
             returned.back().first.push_back(tree_graph.vec_node_node[i].number);
         }
     }
     for(int n=0;n<returned.size();n++){
         int sum_way=0;
         for(int j=0;j<returned[n].first.size()-1;j++){
             if(matrix_weight->data[returned[n].first[j]][returned[n].first[j+1]]==0)
                 qDebug()<<"Error no edge";
             sum_way+=matrix_weight->data[returned[n].first[j]][returned[n].first[j+1]];
         }
         sum_way+=matrix_weight->data[returned[n].first[returned[n].first.size()-1]]
                 [begin_node];
         if(matrix_weight->data[returned[n].first[returned[n].first.size()-1]]
                 [begin_node]==0)
             qDebug()<<"Error no last edge";
         returned[n].second=sum_way;
     }
     return returned;
}


















