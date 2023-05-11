#pragma once
#include<QVector>
#include<QString>
#include<string>
#include<QStandardItemModel>
#include<QDebug>
class Graph;
class Matrix
{
   QVector<QVector<int>> data;
   int Nstring;
   int Mstolb;
public:
    Matrix(QStandardItemModel* model);
    Matrix(QVector<QVector<int>>& data);
    // /////////Правило пяти
    Matrix operator=(const Matrix& other);
    Matrix(const Matrix& other);
    Matrix(Matrix&&other)noexcept;
    Matrix& operator=(Matrix && other)noexcept;
    ~Matrix();
    // //////////////////
    Matrix();

    void Print();
    Matrix operator*(const Matrix& other);
    Matrix MultiplyForShimbel(const Matrix& other, bool flag=0);//flag=1 - поиск максимального пути, 0 - минимального
    friend class Graph;
    friend class MainWindow;
};


