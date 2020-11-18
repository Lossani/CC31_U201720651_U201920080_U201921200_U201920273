#ifndef ADDCONTACTO_H
#define ADDCONTACTO_H

#include <QMainWindow>
#include <contacto.h>
#include <ListaD.h>
#include <fstream>
#include "Arbol.h"


namespace Ui {
class addcontacto;

}

class addcontacto : public QMainWindow
{

    Q_OBJECT

public:

    int limcont;
    int ord = 0;
    QListWidget* lista;

    BST<Contacto,QString>*BST_Cont_CName;
    BST<Contacto,QString>*BST_Cont_Number;
    BST<Contacto,QString>*BST_Cont_Apodo;

    ListaD<Contacto> *ptrLC;
    explicit addcontacto(QWidget *parent = nullptr);
    ~addcontacto();

private slots:
    void btn_volver();
    void btn_agregar();

private:
    Ui::addcontacto *ui;

};

#endif // ADDCONTACTO_H
