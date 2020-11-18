#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "hitagget.h"

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
    User* user = windP.main_instance->getUserByEmail(ui->txtEmail->text().toStdString());

    if (user == nullptr)
    {
        ui->txtPassword->setText("");

        ui->lblerror->setStyleSheet("QLabel { color : red; }");

        ui->lblerror->setText("¡EL USUARIO INGRESADO NO EXISTE!");

        ui->lblerror->show();

        return;
    }

    if (ui->txtEmail->text().toStdString() == user->email)
    {
        if (ui->txtPassword->text().toStdString() == user->password)
        {
            nombre = ui->txtEmail->text();
            apellido = ui->txtEmail->text();

            windP.Unombre = &nombre;
            windP.Uapellido = &apellido;

            windP.cambiar_nombre();

            windP.show();

            Cerrar();
        }
        else
        {
            ui->txtPassword->setText("");

            error_contra--;
            if(error_contra==0) this->close();

            ui->lblerror->setStyleSheet("QLabel { color : red; }");

            ui->lblerror->setText("CONTRASEÑA INCORRECTA, VUELVA A INTENTARLO TIENE "+ QString::number(error_contra) +" INTENTOS");

            ui->lblerror->show();
        }
    }
}

