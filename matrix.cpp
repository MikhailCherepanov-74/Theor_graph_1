#include "matrix.h"

Matrix::Matrix(QStandardItemModel* model){
    Nstring = model->columnCount(); // количество строк в модели
    Mstolb  = model->rowCount();
    QVector<int> help_vec1;
   for (int i = 0; i < Mstolb +1; i++) {
       help_vec1.push_back(0);
   }
     data.push_back(help_vec1);
    for (int i = 0; i < Nstring ; i++) {
        QVector<int> help_vec;
        help_vec.push_back(0);
        for (int j = 0; j < Mstolb; j++) {
           QModelIndex index = model->index(i, j); // индекс элемента в модели
            QVariant data = model->data(index); // данные элемента
            help_vec.push_back(data.toInt());
        }
        data.push_back(help_vec);
    }
}
Matrix::Matrix(){
    Nstring =0;
    Mstolb =0;
    QVector<int> help;
    help.push_back(0);
    data.push_back(help);
}
Matrix::Matrix(QVector<QVector<int>>& data){

    Nstring = data.size()==0?0:data.size()-1;
    if(data.size()!=0)
    {
        Mstolb = data[0].size()==0?0:data[0].size()-1;
    }
    this->data=data;
}

void Matrix::Print(){
    QString help_str="";
    for(int i=1;i<Nstring+1;i++){
        for(int j=1;j<Mstolb+1;j++){
            help_str+=QString::number(data[i][j])+=" ";
        }
        qDebug()<<help_str;
        help_str="";
    }
}
Matrix Matrix:: MultiplyForShimbel(const Matrix &other,bool flag){//flag=1 - поиск максимального пути, 0 - минимального
    if(this->Mstolb!=other.Nstring)
    {
        qDebug()<<"Error multiply";
        return Matrix();
    }
    int new_Mstolb=other.Mstolb;
    int new_Nstring=this->Nstring;
    QVector<int>help_vec;
    for (int i = 0; i < new_Mstolb +1; i++) {
       help_vec.push_back(0);
    }
    QVector<QVector<int>> new_matrix;
    new_matrix.push_back(help_vec);
    for(int k=1;k<=new_Nstring;k++){
        QVector<int>help_vec;
        help_vec.push_back(0);
        int sum=0;
        for(int j=1;j<=other.Mstolb;j++){
            QVector<int> vec_for_choice;
            for(int i=1;i<=this->Mstolb;i++){
                if(this->data[k][i]==0||other.data[i][j]==0)
                    sum=0;
                else{
                    sum=this->data[k][i]+other.data[i][j];
                    vec_for_choice.push_back(sum);
                }
            }
            if(vec_for_choice.empty()){
                 help_vec.push_back(0);
            }
            else{
            if(flag==1)
                sum=*std::max_element(vec_for_choice.begin(), vec_for_choice.end());
            if(flag==0)
                sum=*std::min_element(vec_for_choice.begin(), vec_for_choice.end());
            help_vec.push_back(sum);
            }
            sum=0;
        }
        new_matrix.push_back(help_vec);
    }
    return  Matrix(new_matrix);
}

Matrix Matrix:: operator*(const Matrix &other){
    if(this->Mstolb!=other.Nstring)
    {
        qDebug()<<"Error multiply";
        return Matrix();
    }
    int new_Mstolb=other.Mstolb;
    int new_Nstring=this->Nstring;
    QVector<int>help_vec;
    for (int i = 0; i < new_Mstolb +1; i++) {
       help_vec.push_back(0);
    }
    QVector<QVector<int>> new_matrix;
    new_matrix.push_back(help_vec);
    for(int k=1;k<=new_Nstring;k++){
        QVector<int>help_vec;
        help_vec.push_back(0);
        int sum=0;
        for(int j=1;j<=other.Mstolb;j++){
            for(int i=1;i<=this->Mstolb;i++){
                sum+=this->data[k][i]*other.data[i][j];
            }help_vec.push_back(sum);
            sum=0;
        }
        new_matrix.push_back(help_vec);
    }
    return  Matrix(new_matrix);
}

//             // //////////////Правило пяти
Matrix Matrix::operator=(const Matrix& other){
    this->Mstolb=other.Mstolb;
    this->Nstring=other.Nstring;
    this->data=other.data;
    return *this;
}
Matrix& Matrix::operator=(Matrix && other)noexcept{
    std::swap(Nstring,other.Nstring);
    std::swap(Mstolb,other.Mstolb);
    std::swap(data,other.data);
    other.Nstring = 0;
    other.Mstolb = 0;
    other.data.clear();
}
Matrix::Matrix(const Matrix& other){
    this->Mstolb=other.Mstolb;
    this->Nstring=other.Nstring;
    this->data=other.data;
}
Matrix::Matrix(Matrix&& other) noexcept {
       // Перемещение размеров матрицы
       Mstolb = other.Mstolb;
       Nstring = other.Nstring;

       // Перемещение вектора векторов
       data = std::move(other.data);

       // Установка размеров другого объекта в 0
       other.Nstring = 0;
       other.Mstolb = 0;
       other.data.clear();
   }


Matrix::~Matrix(){

};























