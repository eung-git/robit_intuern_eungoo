#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>

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
    void on_sendButton_clicked();    // 버튼 클릭 시 메시지 전송
    void readPendingDatagrams();     // 수신 처리


private:
    Ui::MainWindow *ui;
    QUdpSocket *udpSocket;
    quint16 localPort = 12347;
};

#endif
