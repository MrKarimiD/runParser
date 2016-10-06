#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_addr_button_clicked();

    void on_process_button_clicked();

    void on_clear_button_clicked();

private:
    Ui::MainWindow *ui;
    QList<QString> files;
    int problemSize;
};

#endif // MAINWINDOW_H
