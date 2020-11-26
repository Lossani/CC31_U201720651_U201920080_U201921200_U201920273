#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QList>
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{


    ui->setupUi(this);

    ui->Ventanas->setCurrentIndex(1);


    QPixmap *img = new QPixmap("Imagenes\\Logo.png");
    ui->lblLogo->setPixmap(*img);
    ui->lblLogo_2->setPixmap(*img);

    ui->lblerrorReg->hide();

    error_contra = 4;

    ui->lblerror->hide();

    connect(ui->btnRegistrar,SIGNAL(released()),this,SLOT(Registrar()));
    connect(ui->btnIngresar,SIGNAL(released()),this,SLOT(Ingresar()));
    connect(ui->btnCrearCuenta,SIGNAL(released()),this,SLOT(Mostrar_Registro()));

    windP.loginWindow = this;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Mostrar_Registro(){
    ui->Ventanas->setCurrentIndex(0);
}

void MainWindow::Cerrar(){
    windP.show_user_info();
    windP.update_contacts();
    //windP.act_tend();
    windP.show();

    ui->leContraReg->setText("");
    ui->leContra_2->setText("");
    ui->leCorreo->setText("");
    ui->leCorreoReg->setText("");
    ui->leNombreReg->setText("");

    ui->Ventanas->setCurrentIndex(1);
    this->hide();
}

void MainWindow::Registrar(){
    if(ui->leNombreReg->text()!="" && ui->leCorreoReg->text() != "" && ui->leContraReg->text() != ""){
        ui->lblerrorReg->hide();

        bool result = windP.main_instance->sign_up(ui->leCorreoReg->text().toStdString() + ui->cbReg->currentText().toStdString(), ui->leNombreReg->text().toStdString(), ui->leContraReg->text().toStdString());

        if (result)
        {
            windP.Unombre = new QString(windP.main_instance->logged_user->fullname.c_str());
            windP.UfechaR = new QString(windP.main_instance->logged_user->registerDate.c_str());
            Cerrar();
        }
        else
        {
            ui->lblerrorReg->setText("ESTE CORREO YA ESTA REGISTRADO");
            ui->lblerrorReg->show();
        }
    } else {
        ui->lblerrorReg->setText("DEBE LLENAR TODOS LOS CAMPOS");
        ui->lblerrorReg->show();
    }
}

void MainWindow::Ingresar(){

    if (ui->leCorreo->text() != "")
    {
        if (!windP.main_instance->log_in(ui->leCorreo->text().toStdString()))
        {
            ui->lblerror->setText("Este correo no se encuentra registrado\nLe recomendamos crear una cuenta en HiTagget");
            ui->lblerror->show();
        }
        else
        {
            windP.Unombre = new QString(windP.main_instance->logged_user->fullname.c_str());
            windP.UfechaR = new QString(windP.main_instance->logged_user->registerDate.c_str());
            Cerrar();
        }
    }
    else
    {
        ui->lblerror->setText("Debe llenar la casilla de correo");
        ui->lblerror->show();
    }
}


void MainWindow::on_btnGoBack_clicked()
{
    ui->Ventanas->setCurrentIndex(1);
}
