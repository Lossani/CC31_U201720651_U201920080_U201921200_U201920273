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

void MainWindow::Registrar(){

    function<void(QList<Usuario>)> SaveText = [](QList<Usuario>lista) {
        ofstream archivo;
        archivo.open("users.tsv");
        if(archivo.good()){
            for(int i = 0; i<lista.size();i++){
                if(i==lista.size()-1){
                    string saveID = lista[i].get_id().toStdString();
                    string saveCorreo = lista[i].get_email().toStdString();
                    string saveName = lista[i].get_fullname().toStdString();
                    string saveDate = lista[i].get_regdate().toStdString();

                  archivo<<saveID<<"\t"<<saveCorreo<<"\t"<<saveName<<"\t"<<saveDate;

                } else{
                    string saveID = lista[i].get_id().toStdString();
                    string saveCorreo = lista[i].get_email().toStdString();
                    string saveName = lista[i].get_fullname().toStdString();
                    string saveDate = lista[i].get_regdate().toStdString();

                  archivo<<saveID<<"\t"<<saveCorreo<<"\t"<<saveName<<"\t"<<saveDate<<endl;

                }

            }
            archivo.close();
        }
        };


    function<bool(QString c)> VerifCorreo = [](QString c){
        bool ver = true;
       for(int i =0;i<c.size();i++){
           if(c[i] == "@"){
               ver = false;
           }
       }
       return ver;
    };


    if(ui->leNombreReg->text()!="" && ui->leCorreoReg->text() != "" && ui->leContraReg->text() != ""){
        ui->lblerrorReg->hide();
        QDateTime current = QDateTime::currentDateTime();
        QString fecha = current.toString("yyyy-MM-dd");

        QString temp = ui->leCorreoReg->text() + ui->cbReg->currentText();
        QString correo_nuevo = "";
        for(int i = 0;i<temp.size()-4;i++){
            correo_nuevo = correo_nuevo + temp[i];
        }


        bool exist_correo = false;

        for(int i=0;i<lverif.size();i++){
            if(lverif[i].get_email() == correo_nuevo){
            exist_correo = true;}
        }

        if(VerifCorreo(ui->leCorreoReg->text())){
            ui->lblerrorReg->hide();
            if(exist_correo){

                ui->lblerrorReg->setText("ESTE CORREO YA ESTA REGISTRADO");
                ui->lblerrorReg->show();

            } else {
                Usuario nuevouser(QString::number(lverif.size()),correo_nuevo,ui->leNombreReg->text(),fecha);
                lverif.append(nuevouser);
                ui->lblerrorReg->hide();

                SaveText(lverif);

                QString nametemp = nuevouser.get_fullname();
                windP.Unombre = &nametemp;

                QString datetemp = nuevouser.get_regdate();
                windP.UfechaR = &datetemp;


                windP.show_user_info();

                windP.show();
                Cerrar();

            }

        } else{
            ui->lblerrorReg->setText("CORREO INVALIDO");
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
            windP.show_user_info();
            windP.show();
            Cerrar();
        }
    }
    else
    {
        ui->lblerror->setText("Debe llenar la casilla de correo");
        ui->lblerror->show();
    }
}

