#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QPen>
#include <QFile>
#include <QTextStream>
#include <cmath>
#include <QPushButton>


void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    QPen pen(Qt::black);
    pen.setWidth(5);
    p.setPen(pen);
    p.setBrush(Qt::black);

    QPoint point_1(pointX, pointY);
    QPoint point_2(pointX + R*std::cos(theta_1),
                   pointY + R*std::sin(theta_1));
    QPoint point_3(point_2.x() + R*std::cos(theta_1+theta_2),
                   point_2.y() + R*std::sin(theta_1+theta_2));
    QPoint point_4(point_3.x() + R*std::cos(theta_1+theta_2+theta_3),
                   point_3.y() + R*std::sin(theta_1+theta_2+theta_3));

    p.drawEllipse(point_1, 5, 5);
    p.drawEllipse(point_2, 5, 5);
    p.drawEllipse(point_3, 5, 5);
    p.drawEllipse(point_4, 5, 5);

    p.drawLine(point_1, point_2);
    p.drawLine(point_2, point_3);
    p.drawLine(point_3, point_4);
}

// ===== InputAngle 구현 =====
InputAngle::InputAngle(QSpinBox* spinBox, double& theta, QObject* parent)
    : QObject(parent), spinBox(spinBox), theta(theta)
{
    setupConnection();
}

void InputAngle::setupConnection() {
    // SpinBox 값이 바뀔 때 theta에 반영
    connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [=](int value){
        theta = static_cast<double>(value) * M_PI / 180.0; // 도(degree) → 라디안 변환
    });
}

// ===== MainWindow 구현 =====
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
      theta_1(0), theta_2(0), theta_3(0),
      pointX(400), pointY(200), R(100),
      movingCW_1(false), movingCCW_1(false),
      movingCW_2(false), movingCCW_2(false),
      movingCW_3(false), movingCCW_3(false),
      speed(0.05)
{
    ui->setupUi(this);
    timer.setInterval(50);

    // === UI 포인터 초기화 ===
    p1_cw      = ui->p1_cw;
    p1_ccw     = ui->p1_ccw;
    p2_cw      = ui->p2_cw;
    p2_ccw     = ui->p2_ccw;
    p3_cw      = ui->p3_cw;
    p3_ccw     = ui->p3_ccw;
    btnSave    = ui->btnSave;
    btnLoad    = ui->btnLoad;
    labelStatus = ui->labelStatus;
    labelStatus_2 = ui->labelStatus_2;
    sliderSpeed = ui->sliderSpeed;
    angleUpdate = ui->angleUpdate;
    spinBox1   = ui->spinBox1;
    spinBox2   = ui->spinBox2;
    spinBox3   = ui->spinBox3;

    // === InputAngle 초기화 (SpinBox → theta 연동) ===
    input1 = new InputAngle(spinBox1, theta_1, this);
    input2 = new InputAngle(spinBox2, theta_2, this);
    input3 = new InputAngle(spinBox3, theta_3, this);

    // === SpinBox 기본 설정 ===
    spinBox1->setRange(-180, 180);
    spinBox2->setRange(-180, 180);
    spinBox3->setRange(-180, 180);
    spinBox1->setValue(0);
    spinBox2->setValue(0);
    spinBox3->setValue(0);


    // === 저장/불러오기 버튼 ===
    connect(btnSave, &QPushButton::clicked, this, &MainWindow::saveState);
    connect(btnLoad, &QPushButton::clicked, this, &MainWindow::loadState);

    // === P1 제어 ===
    connect(p1_cw, &QPushButton::pressed, this, [=](){ movingCW_1 = true; timer.start(); });
    connect(p1_cw, &QPushButton::released, this, [=](){ movingCW_1 = false; });
    connect(p1_ccw, &QPushButton::pressed, this, [=](){ movingCCW_1 = true; timer.start(); });
    connect(p1_ccw, &QPushButton::released, this, [=](){ movingCCW_1 = false; });

    // === P2 제어 ===
    connect(p2_cw, &QPushButton::pressed, this, [=](){ movingCW_2 = true; timer.start(); });
    connect(p2_cw, &QPushButton::released, this, [=](){ movingCW_2 = false; });
    connect(p2_ccw, &QPushButton::pressed, this, [=](){ movingCCW_2 = true; timer.start(); });
    connect(p2_ccw, &QPushButton::released, this, [=](){ movingCCW_2 = false; });

    // === P3 제어 ===
    connect(p3_cw, &QPushButton::pressed, this, [=](){ movingCW_3 = true; timer.start(); });
    connect(p3_cw, &QPushButton::released, this, [=](){ movingCW_3 = false; });
    connect(p3_ccw, &QPushButton::pressed, this, [=](){ movingCCW_3 = true; timer.start(); });
    connect(p3_ccw, &QPushButton::released, this, [=](){ movingCCW_3 = false; });

    // === 타이머: 누르고 있는 동안 자동 회전 ===
    connect(&timer, &QTimer::timeout, this, [=](){
        if (movingCW_1)  theta_1 += speed;
        if (movingCCW_1) theta_1 -= speed;
        if (movingCW_2)  theta_2 += speed;
        if (movingCCW_2) theta_2 -= speed;
        if (movingCW_3)  theta_3 += speed;
        if (movingCCW_3) theta_3 -= speed;

        if (!(movingCW_1 || movingCCW_1 || movingCW_2 || movingCCW_2 || movingCW_3 || movingCCW_3)) {
            timer.stop();
        }

        int deg1 = static_cast<int>(theta_1 * 180.0 / M_PI);
        int deg2 = static_cast<int>(theta_2 * 180.0 / M_PI);
        int deg3 = static_cast<int>(theta_3 * 180.0 / M_PI);

        spinBox1->setValue(deg1);
        spinBox2->setValue(deg2);
        spinBox3->setValue(deg3);
        update();
    });
    connect(angleUpdate, &QPushButton::clicked, this, [=](){
        theta_1 = spinBox1->value() * M_PI / 180.0;
        theta_2 = spinBox2->value() * M_PI / 180.0;
        theta_3 = spinBox3->value() * M_PI / 180.0;
        update(); // 다시 그리기
        labelStatus_2->setText("각도 갱신 완료");
    });

    // === 슬라이더: 속도 조절 ===
    sliderSpeed->setMinimum(1);
    sliderSpeed->setMaximum(20);
    sliderSpeed->setValue(5);
    connect(sliderSpeed, &QSlider::valueChanged, this, [=](int value){
        speed = value * 0.01;
        labelStatus->setText(QString("속도: %1 rad").arg(speed));
    });
}

MainWindow::~MainWindow() {
    delete ui;
}

// ===== 상태 저장 =====
void MainWindow::saveState() {
    QFile file("robot_arm_state.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        labelStatus_2->setText("저장 실패");
        return;
    }

    QTextStream out(&file);

    // 현재 theta 값(라디안)을 degree로 변환해서 저장
    int deg1 = static_cast<int>(theta_1 * 180.0 / M_PI);
    int deg2 = static_cast<int>(theta_2 * 180.0 / M_PI);
    int deg3 = static_cast<int>(theta_3 * 180.0 / M_PI);

    out << deg1 << " " << deg2 << " " << deg3 << "\n";

    file.close();
    labelStatus_2->setText("저장 완료");
}

// ===== 상태 불러오기 =====
void MainWindow::loadState() {
    QFile file("robot_arm_state.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        labelStatus_2->setText("불러오기 실패");
        return;
    }

    QTextStream in(&file);
    int deg1, deg2, deg3;
    in >> deg1 >> deg2 >> deg3;
    file.close();

    // SpinBox 값 갱신 → InputAngle이 theta 자동 반영
    spinBox1->setValue(deg1);
    spinBox2->setValue(deg2);
    spinBox3->setValue(deg3);

    theta_1 = deg1 * M_PI / 180.0;
    theta_2 = deg2 * M_PI / 180.0;
    theta_3 = deg3 * M_PI / 180.0;

    update();
    labelStatus_2->setText("불러오기 완료");
}
