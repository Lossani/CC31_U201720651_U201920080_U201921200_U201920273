#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <principal.h>
#include <iostream>
#include <fstream>
#include <sstream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    int error_contra;
    QString contra;
    MainWindow(QWidget *parent = nullptr);
    Principal windP;
    QString nombre;
    QString apellido;
    ~MainWindow();
private slots:
    void Cerrar();
    void Ingresar();
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
