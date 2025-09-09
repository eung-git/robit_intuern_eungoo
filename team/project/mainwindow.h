#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateBall();
    void on_btnFire_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsEllipseItem *ball;
    QTimer *timer;

    double angle1, angle2, power;
    double vx, vy, vz;
    double x, y, z;
    double g;
    double wallZ;
    double t;

    double baseSize; // 공 기본 크기

    double startX, startY; // 공 시작 위치 (화면 오른쪽 하단 고정)

    QGraphicsRectItem* wallCells[3][3]; // 3x3 벽 셀
};

#endif // MAINWINDOW_H
