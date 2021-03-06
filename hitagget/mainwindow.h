#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <principal.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <QCloseEvent>

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
    void Mostrar_Registro();
    void Registrar();
    void on_btnGoBack_clicked();

private:
    Ui::MainWindow *ui;

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
