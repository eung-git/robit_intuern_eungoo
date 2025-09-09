#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtMath>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , angle1(45), angle2(0), power(100),
      vx(0), vy(0), vz(0), x(0), y(0), z(0), t(0),
      enemyAngle1(0), enemyAngle2(0), enemyPower(0),
      evx(0), evy(0), evz(0), ex(0), ey(0), ez(0), et(0),
      g(9.8), wallZ(50.0)
{
    ui->setupUi(this);

    // QGraphicsScene
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    scene->setSceneRect(-200, -200, 400, 400);

    // 벽 3x3
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

    // 내 공 시작 위치 (오른쪽 하단)
    startX = 150;
    startY = -150;
    ball = scene->addEllipse(-5, -5, 10, 10,
                             QPen(Qt::black), QBrush(Qt::red));
    ball->setPos(startX, startY);

    // 상대 공 시작 위치 (왼쪽 하단)
    enemyX = -150;
    enemyY = -150;
    enemyBall = scene->addEllipse(-5, -5, 10, 10,
                                  QPen(Qt::black), QBrush(Qt::green));
    enemyBall->setPos(enemyX, enemyY);

    // 타이머
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateBall);

    enemyTimer = new QTimer(this);
    connect(enemyTimer, &QTimer::timeout, this, &MainWindow::updateEnemyBall);

    // UDP
    udpSocket = new QUdpSocket(this);

    // 파일 준비
    sendFile = new QFile("send.txt");
    recvFile = new QFile("recv.txt");

    // 수신 파일 초기화
    if (recvFile->open(QIODevice::WriteOnly | QIODevice::Truncate))
        recvFile->close();

    // 파일 체크 타이머
    fileCheckTimer = new QTimer(this);
    connect(fileCheckTimer, &QTimer::timeout, this, &MainWindow::checkReceiveFile);
    fileCheckTimer->start(200);

    // 슬라이더
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

    // 기본값
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
    qDebug() << "발사!" << angle1 << angle2 << power;

    t = 0;
    double rad1 = qDegreesToRadians(angle1);
    double rad2 = qDegreesToRadians(angle2);

    vx = power * qCos(rad1) * qSin(rad2);
    vy = power * qSin(rad1);
    vz = power * qCos(rad1) * qCos(rad2);

    x = y = z = 0;

    ball->setRect(-40, -40, 80, 80);
    ball->setPos(startX, startY);

    timer->start(10);

    // 파일 저장
    if (sendFile->open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream out(sendFile);
        out << angle1 << " " << angle2 << " " << power;
        sendFile->close();
    }

    // UDP 전송
    QByteArray data = QString("%1 %2 %3").arg(angle1).arg(angle2).arg(power).toUtf8();
    udpSocket->writeDatagram(data, QHostAddress("172.100.6.164"), 12345);
}

void MainWindow::updateBall()
{
    x = vx * t;
    y = vy * t - 0.5 * g * t * t;
    z = vz * t;

    if (z >= wallZ) { timer->stop(); return; }

    // 크기 (반비례: 80→10)
    double c = 7.14, K = 571.4;
    double size = K / (z + c);
    if (size > 80) size = 80;
    if (size < 10) size = 10;

    t += 0.03;

    ball->setRect(-size/2, -size/2, size, size);
    ball->setPos(startX + x, startY - y);
}

void MainWindow::updateEnemyBall()
{
    ex = evx * et;
    ey = evy * et - 0.5 * g * et * et;
    ez = evz * et;

    if (ez >= wallZ) { enemyTimer->stop(); return; }

    double c = 7.14, K = 571.4;
    double size = K / (ez + c);
    if (size > 80) size = 80;
    if (size < 10) size = 10;

    et += 0.03;

    enemyBall->setRect(-size/2, -size/2, size, size);
    enemyBall->setPos(enemyX + ex, enemyY - ey);
}

void MainWindow::checkReceiveFile()
{
    if (!recvFile->open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QString content = recvFile->readAll().trimmed();
    recvFile->close();

    if (!content.isEmpty()) {
        QStringList parts = content.split(" ");
        if (parts.size() == 3) {
            enemyAngle1 = parts[0].toDouble();
            enemyAngle2 = parts[1].toDouble();
            enemyPower  = parts[2].toDouble();

            double rad1 = qDegreesToRadians(enemyAngle1);
            double rad2 = qDegreesToRadians(enemyAngle2);

            evx = enemyPower * qCos(rad1) * qSin(rad2);
            evy = enemyPower * qSin(rad1);
            evz = enemyPower * qCos(rad1) * qCos(rad2);

            ex = ey = ez = 0;
            et = 0;

            enemyBall->setRect(-40, -40, 80, 80);
            enemyBall->setPos(enemyX, enemyY);

            enemyTimer->start(10);
        }

        // 초기화
        if (recvFile->open(QIODevice::WriteOnly | QIODevice::Truncate))
            recvFile->close();
    }
}
