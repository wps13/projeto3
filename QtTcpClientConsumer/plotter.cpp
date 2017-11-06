#include "plotter.h"
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <cmath>

using namespace std;

Plotter::Plotter(QWidget *parent) : QWidget(parent)
{

}

void Plotter::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QBrush brush;
    QPen pen;
    int x1,x2,y1,y2;

    brush.setColor(QColor(255,255,100));
    brush.setStyle(Qt::SolidPattern);

    pen.setColor(QColor(255,0,0));

    pen.setWidth(2);

    painter.setBrush(brush);

    painter.setPen(pen);

    painter.drawRect(0,0,width(),height());

    pen.setColor(QColor(255,180,0));
    painter.setPen(pen);

    pen.setColor(QColor(0,0,255));
    painter.setPen(pen);
    x1=0;
    y1=height();



}
