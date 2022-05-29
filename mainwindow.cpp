#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainterPath>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)//QWidget(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    setWindowTitle("Симулятор стрельбы по мишени");
    ui->centralwidget->setLayout(ui->vlInterface);
    valNat = new QIntValidator(1,10000, this);
    valCoord = new QIntValidator(-250, 250, this);

    ui->leXEnter->setValidator(valCoord);
    ui->leYEnter->setValidator(valCoord);
    ui->leRadiusEnter->setValidator(valNat);
    ui->leAmmoEnter->setValidator(valNat);

    aim = new Target(0, 0, this);
    ui->vlInterface->insertWidget(1, aim,0,Qt::AlignCenter);
    hits = 0;
    misses = 0;

    connect(aim, &Target::hitsInc, this, &MainWindow::updateHitsLabel);
    connect(aim, &Target::missesInc, this, &MainWindow::updateMissesLabel);
    connect(aim, &Target::noAmmo, this, &MainWindow::errorMessage);

}

void MainWindow::errorMessage() {
    QMessageBox msg;
    msg.setIcon(QMessageBox::Warning);
    msg.setText("Действие не может быть выполнено");
    msg.setWindowTitle("Ошибка");
    msg.exec();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::parameterInc(int &parameter, QLabel &lb)
{
    ++parameter;
    QString str;
    str.setNum(parameter);
    lb.setText(str);
    aim->setAmmo(aim->getAmmo()-1);
    str.setNum(aim->getAmmo());
    ui->lbLeft->setText(str);
}

void MainWindow::on_pbShot_clicked()
{
    if (!isValid(ui->leXEnter, ui->leYEnter)) {
        errorMessage();
    }
    else {
        QPointF point;
        point.setX(ui->leXEnter->text().toInt());
        point.setY(-ui->leYEnter->text().toInt());
        if (aim->isClipEmpty()) {
            errorMessage();
        }
        else if (aim->isHit(point + aim->getPelletCenter())) {
            updateHitsLabel();
        }
        else {
            updateMissesLabel();
        }
    }
}

void MainWindow::updateHitsLabel() {
    parameterInc(hits,*ui->lbHit);
}
void MainWindow::updateMissesLabel() {
    parameterInc(misses, *ui->lbMiss);
}

void MainWindow::enabledChanging(bool restartLbChange, bool coordLbChange) {
    ui->leRadiusEnter->setEnabled(restartLbChange);
    ui->leAmmoEnter->setEnabled(restartLbChange);

    ui->leXEnter->setEnabled(coordLbChange);
    ui->leYEnter->setEnabled(coordLbChange);
    ui->pbShot->setEnabled(coordLbChange);
}

bool MainWindow::isValid(QLineEdit *le1, QLineEdit *le2)
{
    return (le1->hasAcceptableInput() && le2->hasAcceptableInput());
}

bool MainWindow::isRestarted()
{
    return restarted;
}

void MainWindow::on_pbStart_clicked()
{
    if (!isValid(ui->leRadiusEnter, ui->leAmmoEnter)) {
        errorMessage();
    }
    else if (isRestarted()) {
        enabledChanging(true, false);
        ui->pbStart->setText("Старт");
        restarted = false;
        aim->clearShots();
        aim->setAmmo(0);

        ui->leRadiusEnter->clear();
        ui->leAmmoEnter->clear();

    }
    else {
        ui->lbHit->setText("0");
        ui->lbMiss->setText("0");
        ui->lbLeft->setText("0");
        ui->pbStart->setText("Заново");

        ui->leXEnter->clear();
        ui->leYEnter->clear();

        hits = 0;
        misses = 0;

        aim->setRadius(ui->leRadiusEnter->text().toInt());
        aim->setAmmo(ui->leAmmoEnter->text().toInt());
        enabledChanging(false, true);

        restarted = true;

        QString str;
        str.setNum(aim->getAmmo());
        ui->lbLeft->setText(str);
        aim->update();
    }

}

