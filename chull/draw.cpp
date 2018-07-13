#include "draw.h"
#include "qpainter.h"
#include <QDebug>

draw::draw(QWidget *parent) : QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

void draw::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.begin(this);
    QPen pen(Qt::red);
    pen.setWidth(2);
    painter.setPen(pen);
    QPolygon polygon;

    for(size_t i = 0; i < data.size(); ++i)
    {
        polygon.push_back(data[i]);
    }
    qDebug()<<"Polygon size"<<polygon.size();
    painter.drawPoints(polygon);//zvyraznit body
}

void draw::setData(std::vector<QPoint> &points)
{
    if(!data.empty())
        data.clear();

    data.assign(points.begin(), points.end());
}
