#pragma once

#include <QMainWindow>
#include <QTimer>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// === InputAngle 클래스 ===
class InputAngle : public QObject {
    Q_OBJECT
public:
    InputAngle(QSpinBox* spinBox, double& theta, QObject* parent = nullptr);

private:
    QSpinBox* spinBox;
    double& theta;

    void setupConnection();
};

// === MainWindow 클래스 ===
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void saveState();   // 상태 저장
    void loadState();   // 상태 불러오기

private:
    Ui::MainWindow *ui;
    QTimer timer;

    // 관절 각도
    double theta_1, theta_2, theta_3;

    // 로봇팔 기준점
    int pointX, pointY, R;

    // UI 요소들
    QPushButton *p1_cw;
    QPushButton *p1_ccw;
    QPushButton *p2_cw;
    QPushButton *p2_ccw;
    QPushButton *p3_cw;
    QPushButton *p3_ccw;
    QPushButton *btnSave;
    QPushButton *btnLoad;
    QPushButton *angleUpdate;
    QLabel *labelStatus;
    QLabel *labelStatus_2;
    QSlider *sliderSpeed;
    QSpinBox *spinBox1;
    QSpinBox *spinBox2;
    QSpinBox *spinBox3;

    // 버튼 상태 플래그
    bool movingCW_1, movingCCW_1;
    bool movingCW_2, movingCCW_2;
    bool movingCW_3, movingCCW_3;

    // 회전 속도
    double speed;

    // InputAngle 객체
    InputAngle* input1;
    InputAngle* input2;
    InputAngle* input3;
};
