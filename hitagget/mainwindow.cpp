#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{


    ui->setupUi(this);



        QPixmap *img = new QPixmap("Logo.png");
        ui->lblLogo->setPixmap(*img);



    error_contra = 4;

    ui->lblerror->hide();

    fstream c("Contras.txt");
        string tempc;
        c>>tempc;

        contra = QString::fromStdString(tempc);



    connect(ui->btnIngresar,SIGNAL(released()),this,SLOT(Ingresar()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Cerrar(){
    this->hide();
}

void MainWindow::Ingresar(){

    if(ui->leContra->text() == contra ){
        if(ui->leApellido->text()!="" && ui->leNombre->text()!=""){
            nombre = ui->leNombre->text();
            apellido = ui->leApellido->text();

            windP.Unombre = &nombre;
            windP.Uapellido = &apellido;

            windP.cambiar_nombre();

            windP.show();

            Cerrar();
    }


    } else {
        ui->leContra->setText("");

        error_contra--;
        if(error_contra==0) this->close();

        ui->lblerror->setStyleSheet("QLabel { color : red; }");

        ui->lblerror->setText("CONTRASEÑA INCORRECTA, VUELVA A INTENTARLO TIENE "+ QString::number(error_contra) +" INTENTOS");

        ui->lblerror->show();
    }


}

