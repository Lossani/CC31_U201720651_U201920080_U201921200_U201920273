#include "addcontacto.h"
#include "ui_addcontacto.h"

addcontacto::addcontacto(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::addcontacto)
{
    ui->setupUi(this);
    connect(ui->btnAC,SIGNAL(released()),this,SLOT(btn_agregar()));
    connect(ui->btnVolver,SIGNAL(released()),this,SLOT(btn_volver()));
}

void addcontacto::btn_volver(){
    this->hide();
}

void addcontacto::btn_agregar(){

    function<void(ListaD<Contacto>*)> SaveText = [](ListaD<Contacto>*lista) {
        ofstream archivo;
        archivo.open("C:/Users/ChimueloBlack/Documents/hitagget/Contactos.txt");
        lista->selected = lista->head;
        if(archivo.good()){
            for(int i = 0; i<lista->get_len();i++){
                if(i==lista->get_len()-1){
                    string saveNC = lista->selected->data.get_nombre().toStdString();
                    string saveAC = lista->selected->data.get_apellido().toStdString();
                    string saveNumC = lista->selected->data.get_numero().toStdString();
                    string saveApC = lista->selected->data.get_apodo().toStdString();

                  archivo<<saveNC<<" "<<saveAC<<" "<<saveNumC<<" "<<saveApC;
                  lista->sgt();
                } else{
                    string saveNC = lista->selected->data.get_nombre().toStdString();
                    string saveAC = lista->selected->data.get_apellido().toStdString();
                    string saveNumC = lista->selected->data.get_numero().toStdString();
                    string saveApC = lista->selected->data.get_apodo().toStdString();

                  archivo<<saveNC<<" "<<saveAC<<" "<<saveNumC<<" "<<saveApC<<endl;
                  lista->sgt();
                }

            }
            archivo.close();
        }
        };

    if(ui->leNombre->text()!="" && ui->leApellido->text()!="" && ui->leTelefono->text()!="" && ui->leApodo->text()!=""){
        QString tempNC = ui->leNombre->text();
        QString tempAC = ui->leApellido->text();
        QString tempNumC = ui->leTelefono->text();
        QString tempApC = ui->leApodo->text();


        Contacto nuevo(tempNC, tempAC, tempNumC, tempApC);




        ui->leNombre->setText("");
        ui->leApellido->setText("");
        ui->leTelefono->setText("");
        ui->leApodo->setText("");

        ptrLC->append(nuevo);


        SaveText(ptrLC);

        //PRUEBA

        this->hide();

    }



}


addcontacto::~addcontacto()
{
    delete ui;
}
