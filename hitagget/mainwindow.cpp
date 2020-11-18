#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QList>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{


    ui->setupUi(this);

    ui->Ventanas->setCurrentIndex(1);


        QPixmap *img = new QPixmap("Imagenes\\Logo.png");
        ui->lblLogo->setPixmap(*img);
        ui->lblLogo_2->setPixmap(*img);



    error_contra = 4;

    ui->lblerror->hide();

    fstream c("Contras.txt");
        string tempc;
        c>>tempc;

        contra = QString::fromStdString(tempc);



        QFile file("users.csv");
        if(file.open(QIODevice::ReadOnly)){ //WriteOnly
            QTextStream in(&file); // in << palabras[1] << "\n";
            while(!in.atEnd()){
                QString linea = in.readLine();
                QStringList palabras = linea.split(",");
                Usuario nuevo(palabras[1],palabras[2],palabras[3],palabras[4]);
                lverif.append(nuevo);
            }
            file.close();
        }





    connect(ui->btnIngresar,SIGNAL(released()),this,SLOT(Ingresar()));
    connect(ui->btnCrearCuenta,SIGNAL(released()),this,SLOT(Mostrar_Registro()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Mostrar_Registro(){
    ui->Ventanas->setCurrentIndex(0);
}

void MainWindow::Cerrar(){
    this->hide();
}

void MainWindow::Ingresar(){


    for(int i=0;i<lverif.size();i++){

        QString CSCll = "";


        for(int j=1;j < lverif[i].get_email().size()-1;j++){
            CSCll = CSCll+lverif[i].get_email()[j];
        }


        if(CSCll == ui->leCorreo->text()){
            windP.show();
            Cerrar();

            QString NSCll = "";


            for(int j=1;j < lverif[i].get_fullname().size()-1;j++){
                NSCll = NSCll+lverif[i].get_fullname()[j];
            }

            windP.Unombre = &NSCll;

            windP.cambiar_nombre();

            return;
        }
        else{
            ui->lblerror->show();
        }

    }



    /*
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
    }*/


}

