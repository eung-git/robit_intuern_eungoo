#include "../include/mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostAddress>
#include <QByteArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // UDP 소켓 생성 및 바인드
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(localPort, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

    connect(udpSocket, &QUdpSocket::readyRead, this, &MainWindow::readPendingDatagrams);
}

MainWindow::~MainWindow()
{
    delete ui;
}
 //메시지 전송 함수
void MainWindow::on_sendButton_clicked()
{
    QString message = ui->lineEditMessage->text().trimmed();  // 공백 제거 후 문자열 가져오기
    if (message.isEmpty()) {
        // 입력이 없으면 전송x
        return;
    }

    QByteArray data = ui->lineEditMessage->text().toUtf8();
        QHostAddress destIP(ui->lineEditIP->text());
        quint16 destPort = ui->lineEditPort->text().toUShort();

    // UDP 메시지 전송

    udpSocket->writeDatagram(data, destIP, destPort);

    //  내 메시지 출력: 오른쪽 정렬
    QTextCursor cursor(ui->textBrowser->textCursor());
    cursor.movePosition(QTextCursor::End);

    QTextBlockFormat format;
    format.setAlignment(Qt::AlignRight);  // 오른쪽 정렬
    cursor.insertBlock(format);
    //상대 메시지 출력: 왼쪽 정렬
    QTextCharFormat charFormat;
    charFormat.setForeground(QBrush(Qt::black));  // 글자색
    cursor.insertText(QString("Me: %1").arg(ui->lineEditMessage->text()), charFormat);

    // 입력창 비우기
    ui->lineEditMessage->clear();
}
//메시지 수신 함수
void MainWindow::readPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(int(udpSocket->pendingDatagramSize()));
        udpSocket->readDatagram(datagram.data(), datagram.size());

        // QTextCursor 가져오기
        QTextCursor cursor(ui->textBrowser->textCursor());
        cursor.movePosition(QTextCursor::End);

        // 문단 포맷: 왼쪽 정렬
        QTextBlockFormat format;
        format.setAlignment(Qt::AlignLeft);
        cursor.insertBlock(format);

        // 텍스트 출력 (파란색)
        QTextCharFormat charFormat;
        charFormat.setForeground(QBrush(Qt::blue));
        cursor.insertText(QString("Friend: %1").arg(QString::fromUtf8(datagram)), charFormat);
    }
}

