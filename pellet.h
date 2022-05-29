#ifndef PELLET_H
#define PELLET_H

#include <QWidget>
#include <QPainter>

class Target : public QWidget
{
    Q_OBJECT
public:
    explicit Target(int rad, int ammo, QWidget *parent = nullptr);
    ~Target() = default;

    void setWindowCenter();
    void setRadius(unsigned int radius);
    void setAmmo(unsigned int ammo);

    int getRadius() const;
    int getAmmo() const;
    QPointF getPelletCenter() const;

    void drawPellet(QPainter&);
    void drawShot(QPainter&painter);
    bool isHit(QPointF);
    bool isClipEmpty();
    void clearShots();

private:

    unsigned int radius;
    unsigned int ammo;
    QPointF pelletCenter;
    QPointF startPoint;
    QList<QPointF> list;

    //functions' equations
    bool circut(QPointF cur);
    bool Ox_1(QPointF cur);
    bool Oy_1(QPointF cur);
    bool Ox_2(QPointF cur);
    bool line(QPointF cur);


private slots:
    void resizeEvent(QResizeEvent*) override;
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent* event) override;

signals:
    void hitsInc();
    void missesInc();
    void leftInc();
    void noAmmo();
};

#endif // PELLET_H
