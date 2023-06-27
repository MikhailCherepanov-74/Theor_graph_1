#include "Shape.h"

// ///////////////////////////////////////////////////////////ConnectionItem
ConnectionItem::ConnectionItem(NodeItem*& n1,NodeItem*& n2,int weight,QGraphicsScene *&scene){
    node1=n1;
    node2=n2;
    this->weight=weight;
    line = new QGraphicsLineItem(node1->pos().x()//+node1->boundingRect().width() / 2,
                    ,node1->pos().y() // node1->boundingRect().height() / 2,
                    ,node2->pos().x() //+ node2->boundingRect().width() / 2,
                    ,node2->pos().y()); //+ node2->boundingRect().height() / 2);// не понял почему здесь именно так с высотами
    text_weght=new QGraphicsTextItem(QString::number(weight));
    text_weght->setPos((node1->pos()+node2->pos())/2);
    text_weght->setZValue(-1);
    line->setZValue(-2);
    QPen pen1(Qt::blue); // Создаем перо синего цвета
    pen1.setWidth(2);
    line->setPen(pen1);
    SetPolygonDirection();
    this->scene=scene;
}
ConnectionItem::~ConnectionItem(){

    delete line;
    delete text_weght;
    delete polygon_triangle;
}
void ConnectionItem::SetPolygonDirection(){
    QPointF p1 =line->line().p1(); // начальная точка линии в локальных координатах
    QPointF p2 = line->line().p2(); // конечная точка линии в локальных координатах

    QPointF p1Scene = line->mapToScene(p1); // начальная точка линии в координатах сцены
    QPointF p2Scene = line->mapToScene(p2); // конечная точка линии в координатах сцены

    QPointF vector = p2Scene - p1Scene; // вектор направления линии
    qreal angleRadians = qAtan2(vector.y(), vector.x()); // угол в радианах

    // преобразование из радиан в градусы
    qreal angleDegrees = angleRadians * 180.0 / M_PI;

    // угол повернутой линии относительно оси X в плоскости монитора
    qreal angleRelativeToX = 90.0 - angleDegrees;
    double res_div=sqrt((p1Scene.x()-p2Scene.x())*(p1Scene.x()-p2Scene.x())+(p1Scene.y()-p2Scene.y())*(p1Scene.y()-p2Scene.y()))/25;
    double x_new=-(p2Scene.x()-p1Scene.x())/res_div+p2Scene.x();
    double y_new=-(p2Scene.y()-p1Scene.y())/res_div+p2Scene.y();
    qreal endX = x_new + 15 * cos((angleRelativeToX+30) * M_PI / 180); // Вычисляем конечную координату по оси X
    qreal endY = y_new + 15 * sin((angleRelativeToX+30) * M_PI / 180); // Вычисляем конечную координату по оси Y
    double x_new1=-(p2Scene.x()-p1Scene.x())/(res_div*0.5)+p2Scene.x();
    double y_new1=-(p2Scene.y()-p1Scene.y())/(res_div*0.5)+p2Scene.y();
    double x_new2=-(p2Scene.x()-p1Scene.x())/(res_div*0.8)+p2Scene.x();
    double y_new2=-(p2Scene.y()-p1Scene.y())/(res_div*0.8)+p2Scene.y();


//                                        //////////////////Это просто жесть тут надо пофиксить ничего не понятно


    QGraphicsLineItem* new_line = new QGraphicsLineItem(x_new2,y_new2,x_new1,y_new1);
    qreal currentRotation = new_line->rotation();
    QPointF centerPoint = new_line->boundingRect().center();
    new_line->setTransformOriginPoint(centerPoint);
    new_line->setRotation(currentRotation+90);
    new_line->setLine(new_line->line().p1().x(),new_line->line().p1().y(), new_line->line().p2().x(),new_line->line().p2().y());
    QPointF rotatedP1 = new_line->mapToParent(new_line->line().p1());
    QPointF rotatedP2 = new_line->mapToParent(new_line->line().p2());

    QPolygonF *polygon = new QPolygonF();
    *polygon << rotatedP1 << rotatedP2 << QPointF(x_new,y_new);
    QGraphicsPolygonItem* polygonItem = new QGraphicsPolygonItem(*polygon);
    QPen pen(Qt::black);
    polygonItem->setPen(pen);

    // Создаем кисть заливки
    QBrush brush(m_brush);                           // ////////////////////////// тут

    // Задаем кисть заливки
    polygonItem->setBrush(brush);
    this->polygon_triangle=polygonItem;
}



void ConnectionItem::Select(){
    QPen pen = line->pen(); // получаем текущий объект QPen для линии
    pen.setColor(Qt::red); // устанавливаем новый цвет линии
    line->setPen(pen); // устанавливаем новый объект QPen для линии
    polygon_triangle->setPen(pen);
    polygon_triangle->setBrush(Qt::red);
    m_brush=Qt::red;
    selected=1;
}
void ConnectionItem::Deselect(){
    QPen pen = line->pen(); // получаем текущий объект QPen для линии
    pen.setColor(Qt::blue); // устанавливаем новый цвет линии
    line->setPen(pen); // устанавливаем новый объект QPen для линии
    polygon_triangle->setPen(pen);
    polygon_triangle->setBrush(Qt::blue);
    m_brush=Qt::blue;
    selected=0;
}
void ConnectionItem::AddCost(int cost){
    QString str_old=text_weght->toPlainText();
    int spaceIndex = str_old.indexOf(" "); // Поиск позиции первого пробела
    str_old = str_old.left(spaceIndex);
    QString str_new=str_old+" ("+QString::number(cost)+")";
    text_weght->setPlainText(str_new);
}
void ConnectionItem::DeleteCost(){
    QString str_old=text_weght->toPlainText();
    int spaceIndex = str_old.indexOf(" "); // Поиск позиции первого пробела
    QString firstWord = str_old.left(spaceIndex);
    text_weght->setPlainText(firstWord);
}
// /////////////////////////////////////////////////////////////////////////////NodeIten
unsigned int NodeItem::Z_value=0;

void NodeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    /* При отпускании мышью элемента
     * заменяем на обычный курсор стрелку
     * */
    this->setCursor(QCursor(Qt::ArrowCursor));
    Q_UNUSED(event);
}

void NodeItem::mousePressEvent(QGraphicsSceneMouseEvent* event)                             //Move не работает без pres
{
    /* При нажатии мышью на графический элемент
     * заменяем курсор на руку, которая держит этот элемента
     * */
    Z_value++;
    this->setPos(mapToScene(event->pos()));
    this->setZValue(Z_value);
    this->setCursor(QCursor(Qt::ClosedHandCursor));
    Q_UNUSED(event);
}

void NodeItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    /* Устанавливаем позицию графического элемента
     * в графической сцене, транслировав координаты
     * курсора внутри графического элемента
     * в координатную систему графической сцены
     * */
    Z_value++;
    this->setPos(mapToScene(event->pos()));
    this->setZValue(Z_value);
    for(int i=0;i<vector_connection.size();i++){
        vector_connection[i]->line->setLine(
                    vector_connection[i]->node1->pos().x(),vector_connection[i]->node1->pos().y(),
                    vector_connection[i]->node2->pos().x(),vector_connection[i]->node2->pos().y());
         vector_connection[i]->text_weght->setPos( (vector_connection[i]->node1->pos()+ vector_connection[i]->node2->pos())/2);
            scene->removeItem(vector_connection[i]->polygon_triangle);
         vector_connection[i]->SetPolygonDirection();
         scene->addItem(vector_connection[i]->polygon_triangle);

    }
}

void NodeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
  //  QGraphicsTextItem* textItem = new QGraphicsTextItem("1");
   // textItem->setPos(0, 0);
    QPen pen(QColor(60, 60, 60));
    pen.setWidth(3);
    painter->setPen(pen);
    painter->setBrush(m_brush);
    painter->drawEllipse(QRect(-25, -25, 50,50));
    painter->setFont( QFont("Arial", 20));
    painter->drawText(-10, 5, QString::number(this->number));
}


void NodeItem::Select(){
    this->setBrush(QColor(250, 20, 190)); // получаем текущий объект QPen для линии
    selected=1;
}
void NodeItem::Deselect(){
    this->setBrush(QColor(230, 210, 190)); // получаем текущий объект QPen для линии
    selected=1;
}

QRectF NodeItem::boundingRect() const {
    return QRectF(-25, -25, 50,50);
}

NodeItem::NodeItem(int num,int x1,int y1,QGraphicsScene *&scene){
    number=num;
    x=x1;
    y=y1;
    this->scene=scene;

}
void NodeItem::NumberMinusOne(){
    if(number!=1)
        number--;
}
void NodeItem::AddConnection(ConnectionItem *&connection){
    vector_connection.push_back(connection);
}
 NodeItem::~NodeItem(){

 };
