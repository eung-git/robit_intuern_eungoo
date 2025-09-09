#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QTimer>
#include <QUdpSocket>
#include <QFile>

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
    void updateEnemyBall();
    void on_btnFire_clicked();
    void checkReceiveFile();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsEllipseItem *ball;       // 빨간 공
    QGraphicsEllipseItem *enemyBall;  // 초록 공
    QTimer *timer;      // 내 공
    QTimer *enemyTimer; // 상대 공
    QTimer *fileCheckTimer; // 파일 체크

    QUdpSocket *udpSocket;
    QFile *sendFile;
    QFile *recvFile;

    // 내 공 변수
    double angle1, angle2, power;
    double vx, vy, vz;
    double x, y, z;
    double t;

    // 상대 공 변수
    double enemyAngle1, enemyAngle2, enemyPower;
    double evx, evy, evz, ex, ey, ez, et;

    // 환경 변수
    double g;
    double wallZ;

    // 시작 위치
    double startX, startY;     // 빨간 공 시작 위치
    double enemyX, enemyY;     // 초록 공 시작 위치

    QGraphicsRectItem* wallCells[3][3]; // 벽 셀
};

#endif // MAINWINDOW_H
