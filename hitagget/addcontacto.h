#ifndef ADDCONTACTO_H
#define ADDCONTACTO_H

#include <QMainWindow>
#include <contacto.h>
#include <ListaD.h>
#include <fstream>


namespace Ui {
class addcontacto;

}

class addcontacto : public QMainWindow
{

    Q_OBJECT

public:

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
