#ifndef PRINCIPAL_H
#define PRINCIPAL_H

#include <QMainWindow>
#include <QListWidget>
#include <QCloseEvent>
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

    QMainWindow* loginWindow;

    explicit Principal(QWidget *parent = nullptr);
    ~Principal();

public:
    Hitagget *main_instance = nullptr;
    void show_all_posts(int op, bool inv, bool show_specific_profile);
    void act_tend();
    void show_user_info();

private slots:
    void act_cont();
    void act_cont_ANom();
    void act_cont_ANum();
    void act_cont_AAp();

    void cambiar_imagen();
    void new_publi();
    void add_contact();

    void f_invertir();;

    void on_txtSearchBox_returnPressed();

    void on_cb_criterios_currentIndexChanged(int index);

    void on_btnCloseUserProfile_clicked();

    void on_btnFollow_clicked();

    void on_btnLogOut_clicked();

    void on_btnShowMyProfile_clicked();

private:
    void add_item_to_list_widget(QListWidget *list, Post* individual_post, function<void(Post*, string)> show_post, function<void(int)> show_author_profile, string author_name);
    void show_followed_users();
    Ui::Principal *ui;

protected:
     void closeEvent(QCloseEvent *event);
};

#endif // PRINCIPAL_H
