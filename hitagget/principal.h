#ifndef PRINCIPAL_H
#define PRINCIPAL_H

#include <QMainWindow>
#include <QListWidget>
#include <QCloseEvent>

#include <QQueue>

#include <iostream>
#include <fstream>
#include <sstream>
#include <QTextStream>
#include <clocale>

#include <hitagget.h>

namespace Ui {
class Principal;
}

class Principal : public QMainWindow
{
    Q_OBJECT

public:
    string *ruta_foto;

    QString *Unombre;
    QString *UfechaR;

    QMainWindow* loginWindow;

    explicit Principal(QWidget *parent = nullptr);
    ~Principal();

public:
    Hitagget *main_instance = nullptr;
    void show_all_posts(int op, bool inv, bool show_specific_profile);
    void show_user_info();
    void update_contacts();
    void act_tend();

private slots:
    void cambiar_imagen();
    void new_publi();

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
    void show_contacts(list<User*> contacts);

    Ui::Principal *ui;

    bool invertir = false;
    int op_busq = 0;

protected:
     void closeEvent(QCloseEvent *event);
};

#endif // PRINCIPAL_H
