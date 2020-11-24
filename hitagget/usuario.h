#ifndef USUARIO_H
#define USUARIO_H
#include <QApplication>
#include <string>
using namespace std;

class Usuario{
private:
    QString email;
    QString fullname;
    QString registerDate;
    QString password;
    QString id;
public:

    Usuario(){
        email = "";
        fullname = "";
        registerDate = "";
        password = "";
        id = "";
    }
    Usuario(QString idd, QString em, QString f, QString rd){
        email = em;
        fullname = f;
        registerDate = rd;
        id = idd;
    }

    ~Usuario(){
    }

    QString get_id(){return id;}
    QString get_email(){return email;}
    QString get_fullname(){return fullname;}
    QString get_regdate(){return registerDate;}
    QString get_pass(){return password;}

    void set_id(QString idd){id = idd;}
    void set_email(QString em){email = em;}
    void set_fullnanme(QString f){fullname = f;}
    void set_regdate(QString rd){registerDate = rd;}
    void set_pass(QString pass){password = pass;}
};

#endif // USUARIO_H
