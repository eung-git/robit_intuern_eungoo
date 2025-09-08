#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QFile>
#include <QTextStream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class TextSave {
public:
    static void saveFile(const QString &text);
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int number=0;
    QString txt_re, txt, txt_s;

private slots:
    void on_btnSwitch_clicked();   // 버튼 눌렀을 때 호출
    void on_e11_clicked();         // 버튼 눌렀을 때 호출
    void on_back_clicked();
    void on_space_bar_clicked();
    void on_Enter_clicked();
    void on_Save_clicked();

private:
    QTimer *cursorTimer;
    bool cursorVisible = false;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
