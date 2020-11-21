#ifndef PRINCIPAL_H
#define PRINCIPAL_H

#include <QMainWindow>
#include <QListWidget>
#include <ListaD.h>
#include <QQueue>
#include <contacto.h>
#include <publicacion.h>
#include <grupo.h>
#include <ListaT.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <addcontacto.h>
#include <QTextStream>
#include <clocale>
#include <usuario.h>

#include <Arbol.h>
#include <hitagget.h>

namespace Ui {
class Principal;
}

class Principal : public QMainWindow
{
    Q_OBJECT

public:
    BST<Contacto,QString>*BST_Cont_CName;
    BST<Contacto,QString>*BST_Cont_Number;
    BST<Contacto,QString>*BST_Cont_Apodo;
    int limcont;

    QQueue<Hitagged*> Publi;
    ListaD<Contacto> lcont;
    ListaT *lst;

    struct tnd{
        QString hash;
        int cant;
        tnd(QString h, int c): hash(h), cant(c) {}
    };


    Hitagged *new_public;
    string *ruta_foto;

    Usuario *usuario;

    QString *Unombre;
    QString *UfechaR;


    addcontacto ventAC;

    explicit Principal(QWidget *parent = nullptr);
    ~Principal();

public:
    Hitagget *main_instance = nullptr;
    void show_all_posts();
    void act_tend();
    void cambiar_nombre();

private slots:
    void act_cont();
    void act_cont_ANom();
    void act_cont_ANum();
    void act_cont_AAp();

    void cambiar_imagen();
    void new_publi();
    void add_contact();

private:
    void add_item_to_list_widget(QListWidget *list, Post individual_post, function<void(int)> show_post);
    Ui::Principal *ui;
};

#endif // PRINCIPAL_H
