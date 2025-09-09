#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtMath>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , angle1(45), angle2(0), power(100),
      vx(0), vy(0), vz(0), x(0), y(0), z(0),
      g(9.8), wallZ(50.0), t(0.0), baseSize(10.0)
{
    ui->setupUi(this);

    // QGraphicsScene 설정
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    scene->setSceneRect(-200, -200, 400, 400);

    // 벽 (3x3 격자)
    int cellW = 100;
    int cellH = 50;
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            int x0 = -150 + col * cellW;
            int y0 = -150 + row * cellH;
            wallCells[row][col] = scene->addRect(
                x0, y0, cellW, cellH,
                QPen(Qt::black), QBrush(Qt::white));
        }
    }

    // 공 시작 위치 (오른쪽 하단)
    startX = 150;
    startY = 150;

    // 공
    ball = scene->addEllipse(-baseSize/2, -baseSize/2, baseSize, baseSize,
                             QPen(Qt::black), QBrush(Qt::red));
    ball->setPos(startX, startY);

    // 타이머
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateBall);

    // 슬라이더 값 연결
    connect(ui->sliderAngle1, &QSlider::valueChanged, this, [=](int val){
        angle1 = val;
        ui->labelAngle1->setText(QString("각도1: %1°").arg(val));
    });
    connect(ui->sliderAngle2, &QSlider::valueChanged, this, [=](int val){
        angle2 = val;
        ui->labelAngle2->setText(QString("각도2: %1°").arg(val));
    });
    connect(ui->sliderPower, &QSlider::valueChanged, this, [=](int val){
        power = val;
        ui->labelPower->setText(QString("파워: %1").arg(val));
    });

    // 기본값 세팅
    ui->sliderAngle1->setRange(0, 90);
    ui->sliderAngle1->setValue(45);
    ui->sliderAngle2->setRange(-90, 90);
    ui->sliderAngle2->setValue(0);
    ui->sliderPower->setRange(10, 200);
    ui->sliderPower->setValue(100);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnFire_clicked()
{
    qDebug() << "발사!" << "각도1 =" << angle1 << "각도2 =" << angle2 << "파워 =" << power;

    // 시간 초기화
    t = 0.0;

    // 각도 → 라디안
    double rad1 = qDegreesToRadians(angle1);
    double rad2 = qDegreesToRadians(angle2);

    // 속도 분해
    vx = power * qCos(rad1) * qSin(rad2); // 좌우
    vy = power * qSin(rad1);              // 위
    vz = power * qCos(rad1) * qCos(rad2); // 전방

    // 시작 좌표
    x = 0; y = 0; z = 0;

    // 공 초기 크기 및 위치
    ball->setRect(-baseSize/2, -baseSize/2, baseSize, baseSize);
    ball->setPos(startX, startY);

    // 애니메이션 시작
    timer->start(5);
}

void MainWindow::updateBall()
{
    // 3D 위치 계산
    x = vx * t;
    y = vy * t - 0.5 * g * t * t;
    z = vz * t;

    // 땅에 닿으면 멈춤 (startY 기준 아래로 내려가면 멈춤)
    if (startY - y < -200) {
        timer->stop();
        return;
    }

    // 벽에 닿으면 멈춤
    if (z >= wallZ*10) {
        timer->stop();

        // 맞은 위치 → 벽 좌표계 변환
        int cellW = 100;
        int cellH = 50;

        int col = (startX + x - 150) / cellW;
        int row = (150 - (startY - y)) / cellH;

        if (row >= 0 && row < 3 && col >= 0 && col < 3) {
            wallCells[row][col]->setBrush(QBrush(Qt::red));
        }

        return;
    }

    // 원근 크기
    double scale = wallZ / (t*z/(80-10)) ;
    double size = baseSize * scale;

    t += 0.03;

    // 공 위치 (정면 시점: x,y만 표시 + 시작위치 보정)
    ball->setRect(-size/2, -size/2, size, size);
    ball->setPos(startX + x, startY - y);
}
