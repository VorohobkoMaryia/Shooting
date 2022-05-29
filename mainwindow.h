#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <pellet.h>
#include <QPainter>
#include <QMouseEvent>
#include <QLabel>
#include <QIntValidator>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void parameterInc(int &parameter, QLabel &lb);
    void createPellet();
    void enabledChanging(bool rad_ammo_lb_enabled, bool coord_lb_enabled);
    bool isValid(QLineEdit *le1, QLineEdit *le2);
    bool isRestarted();

private:
    Target* aim;
    QIntValidator* valNat;
    QIntValidator* valCoord;
    int hits;
    int misses;
    bool restarted = false;
    Ui::MainWindow *ui;

private slots:
    void on_pbShot_clicked();
    void updateHitsLabel();
    void updateMissesLabel();
    void errorMessage();
    void on_pbStart_clicked();
};
#endif // MAINWINDOW_H
