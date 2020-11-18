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
        archivo.open("Contactos.csv");
        lista->selected = lista->head;
        if(archivo.good()){
            for(int i = 0; i<lista->get_len();i++){
                if(i==lista->get_len()-1){
                    string saveNC = lista->selected->data.get_comp_name().toStdString();
                    string saveNumC = lista->selected->data.get_numero().toStdString();
                    string saveApC = lista->selected->data.get_apodo().toStdString();

                  archivo<<saveNC<<","<<saveNumC<<","<<saveApC;
                  lista->sgt();
                } else{
                    string saveNC = lista->selected->data.get_comp_name().toStdString();
                    string saveNumC = lista->selected->data.get_numero().toStdString();
                    string saveApC = lista->selected->data.get_apodo().toStdString();

                  archivo<<saveNC<<","<<saveNumC<<","<<saveApC<<endl;
                  lista->sgt();
                }

            }
            archivo.close();
        }
        };

    if(ui->leNombre->text()!="" && ui->leApellido->text()!="" && ui->leTelefono->text()!="" && ui->leApodo->text()!=""){
        QString tempNC = ui->leNombre->text() +" "+ ui->leApellido->text();
        QString tempNumC = ui->leTelefono->text();
        QString tempApC = ui->leApodo->text();


        Contacto nuevo(tempNC, tempNumC, tempApC);

        BST_Cont_Apodo->add(nuevo);
        BST_Cont_CName->add(nuevo);
        BST_Cont_Number->add(nuevo);

        lista->clear();

        switch(ord){
        case 0: BST_Cont_Apodo->inorder(lista,limcont);break;
        case 1: BST_Cont_Apodo->postorder(lista,limcont);break;
        case 2: BST_Cont_CName->inorder(lista,limcont);break;
        case 3: BST_Cont_CName->postorder(lista,limcont);break;
        case 4: BST_Cont_Number->inorder(lista,limcont);break;
        case 5: BST_Cont_Number->postorder(lista,limcont);break;
        }




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
