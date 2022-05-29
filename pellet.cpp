#include "pellet.h"
#include <QPainterPath>
#include <QDebug>
#include <QPixmap>
#include <QMouseEvent>
#include <QMessageBox>

Target::Target(int rad, int ammunition, QWidget *parent)
    : QWidget{parent}
{
    radius = rad;
    ammo = ammunition;
    setMinimumSize(QSize(500,500));
    setWindowCenter();
}

void Target::mousePressEvent(QMouseEvent *event)
{
    if (isClipEmpty()) {
        emit noAmmo();
    }
    else if (isHit(event->pos())) {
        emit hitsInc();
    }
    else {
        emit missesInc();
    }
}

bool Target::isClipEmpty() {
    return ammo == 0;
}

void Target::clearShots()
{
    list.clear();
}

bool Target::isHit(QPointF shot)
{
    list.push_back(shot);
    update();
    return (Oy_1(shot) &&Ox_1(shot) &&line(shot))
            || (circut(shot) && Ox_2(shot));
}

void Target::setWindowCenter()
{
    pelletCenter = QPoint(width()/2,height()/2);
}

void Target::setAmmo(unsigned int ammo)
{
    this->ammo = ammo;
}

int Target::getRadius() const
{
    return radius;
}

int Target::getAmmo() const
{
    return ammo;
}

QPointF Target::getPelletCenter() const
{
    return pelletCenter;
}

void Target::setRadius(unsigned int radius)
{
    this->radius = radius;
}


void Target::resizeEvent(QResizeEvent *)
{
    setWindowCenter();
    update();
}

void Target::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    drawPellet(painter);
    drawShot(painter);
}

void Target::drawPellet(QPainter& painter) {
    //field
    painter.setPen(QPen(QBrush("#fff9f2"), 0));
    painter.setBrush(QBrush("#fff9f2"));
    painter.drawRect(painter.window());

    //pie
    painter.setBrush(QBrush("#BBBBBB"));
    painter.setPen(QPen(QBrush("#BBBBBB"), 0));
    painter.setRenderHint(QPainter::Antialiasing);
    QRectF rectangle(pelletCenter.x(), pelletCenter.y(),2*radius,2*radius);
    startPoint.setX(painter.window().x());
    startPoint.setY(painter.window().y());
    rectangle.moveCenter(pelletCenter);
    painter.drawPie(rectangle, 0 * 16, 180 * 16);

    //triangle
    QPainterPath path;
    path.moveTo (pelletCenter);
    path.lineTo ( pelletCenter.x()-radius, pelletCenter.y() + radius);
    path.lineTo (pelletCenter.x() , pelletCenter.y()+radius);
    path.lineTo (pelletCenter);
    painter.fillPath (path, QBrush("#BBBBBB"));

    //lines
    painter.setPen(QPen(QBrush("grey"), 1));
    QPointF pointsX[4] = {
        pelletCenter + QPointF(148, 3),
        pelletCenter + QPointF(148, -3),
        pelletCenter + QPointF(151,0),
        pelletCenter + QPointF(148, 3),
    };
    painter.drawPolygon(pointsX, 4);

    QPointF pointsY[4] = {
        pelletCenter + QPointF(-3, -148),
        pelletCenter + QPointF(3, -148),
        pelletCenter + QPointF(0,-151),
        pelletCenter + QPointF(-3, -148),
    };

    painter.drawText(pelletCenter + QPointF(155, 2), tr("x"));
    painter.drawText(pelletCenter - QPointF(2, 157), tr("y"));
    painter.drawText(pelletCenter - QPointF(-4, 4), tr("0"));
    painter.drawPolygon(pointsY, 4);
    painter.drawLine(pelletCenter - QPointF(150,0), pelletCenter + QPointF(150,0));
    painter.drawLine(pelletCenter - QPointF(0, 150), pelletCenter + QPointF(0, 150));

}
void Target::drawShot(QPainter& painter) {
    QPen pen;
    pen.setColor("red");
    pen.setWidth(3);
    painter.setPen(pen);
    painter.setRenderHint(QPainter::Antialiasing);
    for (int i = 0; i < list.size(); i++) {
        painter.drawPoint(list[i]);
    }
}

bool Target::circut(QPointF cur)
{
    return (pow(cur.x() - pelletCenter.x(), 2) + pow(cur.y() - pelletCenter.y(), 2)) - radius*radius <= 0;
}

bool Target::Ox_1(QPointF cur)
{
    return (cur.y()-pelletCenter.y() - radius) <= 0;
}

bool Target::Oy_1(QPointF cur)
{
    return cur.x() - pelletCenter.x() <= 0;
}

bool Target::Ox_2(QPointF cur)
{
    return cur.y() - pelletCenter.y() <= 0;
}


bool Target::line(QPointF cur)
{
    return (cur.x()-pelletCenter.x()) + (cur.y()-pelletCenter.y()) >= 0;
}
