#pragma once
//#include "mainwindow.h"
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>
#include <QResizeEvent>
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsTextItem>
#include <QList>
#include <QGraphicsLineItem>
#include <algorithm>
#include <QtMath>
#include <QtDebug>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
class ConnectionItem;
class MainWindow;
class NodeItem :public QGraphicsItem
{
    int number=1;
    int x,y;
    bool selected=0;
    QGraphicsScene *scene;
    QVector<ConnectionItem*> vector_connection;
    QVector<ConnectionItem*> vector_connection_come_here;
    QBrush m_brush=QColor(230, 210, 190);
public:
    static void ZValueToZero() {
        Z_value = 0;
    };


    QRectF boundingRect() const override;

    static unsigned int Z_value;
    void  mousePressEvent(QGraphicsSceneMouseEvent* event);
    void  mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    void  mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    QPainterPath shape() const
    {
        QPainterPath path;
        path.addEllipse(QRect(-25, -25, 50, 50));
        return path;
       // return path;
    }
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
public:
    NodeItem(int num,int x1,int y1,QGraphicsScene *&scene);
    void AddConnection(ConnectionItem *&);
    void NumberMinusOne();
    void Select();
    void Deselect();
    ~NodeItem();
    void setBrush(const QBrush& brush)
    {
        m_brush = brush;
        update();
    }
    friend class MainWindow;
    friend class ConnectionItem;
};



class ConnectionItem{
NodeItem* node1;
NodeItem* node2;
bool selected=0;
int weight=10;
QGraphicsLineItem *line;
QGraphicsPolygonItem *polygon_triangle;
QGraphicsTextItem * text_weght;
QGraphicsScene *scene;
QBrush m_brush=Qt::blue;
public:
ConnectionItem(NodeItem*& n1,NodeItem*& n2,int weight,QGraphicsScene *&scene);
~ConnectionItem();
friend class MainWindow;
friend class NodeItem;
void Select();
void Deselect();
void SetPolygonDirection();
};

//class ConnectionItem




