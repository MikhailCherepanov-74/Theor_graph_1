#include "tree.h"
Tree::Tree(int root)
{
    this->root.number=root;
    this->vec_int_node.push_back(root);
    this->vec_node_node.push_back(this->root);
}
bool Tree::AddNode(int child,int parent){
    if(child==parent){
        qDebug()<<"Error, parent == child";
        return 0;
    }
    //int index_parent=-1;
    QVector<int>vec_index;
    for(int i=0;i<vec_int_node.size();i++){
        if(vec_int_node[i]==parent)
            vec_index.push_back(i);
    }
    bool flag_add=0;
    while(!vec_index.empty()){
        int index_parent=vec_index.front();
        vec_index.pop_front();
        QVector<int> vec_f=vec_node_node[index_parent].vec_parents;
        QVector<int> vec_f_ch=vec_node_node[index_parent].vec_int_children;
        if(std::find(vec_f.begin(), vec_f.end(), child)!=vec_f.end()){
            continue;
        }
        if(std::find(vec_f_ch.begin(), vec_f_ch.end(), child)!=vec_f_ch.end()){
            continue;
        }
        Node node_child;
        node_child.number=child;
        node_child.number_parent=parent;
        node_child.node_parent=&vec_node_node[index_parent];
        node_child.vec_parents=node_child.node_parent->vec_parents;
        node_child.vec_parents.push_back(parent);
        vec_int_node.push_back(child);
        vec_node_node.push_back(node_child);
        flag_add=1;
        vec_node_node[index_parent].vec_int_children.push_back(child);
    }

    return flag_add;
}
