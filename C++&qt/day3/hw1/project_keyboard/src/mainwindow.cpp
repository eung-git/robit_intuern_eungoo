#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTimer>

void TextSave::saveFile(const QString &text) {
    QFile file("/home/eung/output.txt");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << text << "\n";
        file.close();
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->labelOutput->setStyleSheet("color: black; background-color: white;");

    // 타이머 생성
    cursorTimer = new QTimer(this);
    cursorTimer->setInterval(500); // 0.5초마다 실행

    connect(cursorTimer, &QTimer::timeout, this, [=]() {
        QString text = ui->labelOutput->text();
        if (cursorVisible) {
            if (text.endsWith("_")) {
                text.chop(1);
                ui->labelOutput->setText(text);
            }
            cursorVisible = false;
        } else {
            ui->labelOutput->setText(text + "_");
            cursorVisible = true;
        }
    });
    cursorTimer->start();

    // 시작: 한글 위젯 보이게
    ui->widgetKorean->move(400, 300);
    ui->widgetEnglish->move(400, 300);
    ui->widgetEnglish_2->move(400, 300);
    ui->widgetKorean->show();
    ui->widgetEnglish->hide();
    ui->widgetEnglish_2->hide();

    QPushButton *buttons[] = {
        ui->e1, ui->e2, ui->e3, ui->e4, ui->e5, ui->e6,
        ui->e7, ui->e8, ui->e9, ui->e11, ui->e12, ui->e13,
        ui->E2, ui->E3, ui->E4, ui->E5, ui->E6,
        ui->E7, ui->E8, ui->E9
    };

    for (QPushButton *btn : buttons) {
        connect(btn, &QPushButton::clicked, this, [=]() {
            if (btn == ui->back || btn == ui->Enter || btn == ui->space_bar || btn == ui->e11) {
                return;
            }
            QString current = ui->labelOutput->text();
            txt = btn->text();
            if(current.length()-1>=0 && current[current.length()-1]=='_'){
                current.chop(1);
                ui->labelOutput->setText(current);
            }
            if(txt.length()==1){
                ui->labelOutput->setText(current + txt[0]);
                return;
            }

            if(txt_re[0]==txt[0]&&!txt.isEmpty()&&txt_re[0]!="\n"){
                number++;
                if(txt[number]==0)number=0;
                QChar firstChar = txt[number];
                current.chop(1);
                ui->labelOutput->setText(current + QString(firstChar));
            }
            else if (txt_re[0]!=txt[0]&&!txt.isEmpty()&&txt_re[0]!="\n") {
                number=0;
                QChar firstChar = txt[number];
                ui->labelOutput->setText(current + QString(firstChar));
            }
            txt_re = btn->text();
        });

        connect(btn, &QPushButton::pressed, this, [=]() {
            btn->setStyleSheet("background-color: lightblue;");
        });
        connect(btn, &QPushButton::released, this, [=]() {
            btn->setStyleSheet("");
        });
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_e11_clicked()
{
    if (ui->widgetEnglish_2->isVisible()) {
        ui->widgetEnglish->show();
        ui->widgetEnglish_2->hide();
    }
    else {
        ui->widgetEnglish_2->show();
        ui->widgetEnglish->hide();
    }
}

void MainWindow::on_back_clicked()
{
    QString current = ui->labelOutput->text();
    if(current.length()!=0&&current[current.length()-1]=='_'){
        current.chop(1);
    }
    if (!current.isEmpty()) {
        current.chop(1);
        ui->labelOutput->setText(current);
    }
}

void MainWindow::on_space_bar_clicked()
{
    QString current = ui->labelOutput->text();
    if(current.length()!=0&&current[current.length()-1]=='_'){
        current.chop(1);
    }
    if(txt_re[0]==' '){
        ui->labelOutput->setText(current + ' ');
        number=0;
        txt_re=' ';
    }
    else {
        number=0;
        txt_re=" ";
    }

}

void MainWindow::on_Enter_clicked()
{
    QString current = ui->labelOutput->text();
    if(current.endsWith("_")){
        current.chop(1);
    }
    ui->labelOutput->setText(current + '\n');
    number=0;
    txt_re=" ";
}
void MainWindow::on_Save_clicked()
{


    QString current = ui->labelOutput->text();
    if(current.isEmpty())return;

    if(current.length()!=0&&current[current.length()-1]=='_'){
        current.chop(1);
    }

    TextSave::saveFile(current);
    ui->labelOutput->setText("");
}




void MainWindow::on_btnSwitch_clicked()
{
    if (ui->widgetKorean->isVisible()) {
        ui->widgetEnglish->show();
        ui->widgetKorean->hide();
        ui->e1->move(420,310);
        ui->e11->show();
        ui->e12->show();
        ui->e13->show();
    }
    else {
        ui->widgetKorean->show();
        ui->widgetEnglish->hide();
        ui->widgetEnglish_2->hide();
        ui->e1->move(690,410);
        ui->e11->hide();
        ui->e12->hide();
        ui->e13->hide();
    }
}
