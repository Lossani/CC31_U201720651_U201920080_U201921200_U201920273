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
public:

    Usuario(){
        email = "";
        fullname = "";
        registerDate = "";
        password = "";
    }
    Usuario(QString em, QString f, QString rd, QString pass){
        email = em;
        fullname = f;
        registerDate = rd;
        password = pass;
    }

    ~Usuario(){
    }

    QString get_email(){return email;}
    QString get_fullname(){return fullname;}
    QString get_regdate(){return registerDate;}
    QString get_pass(){return password;}

    void get_email(QString em){email = em;}
    void get_fullnanme(QString f){fullname = f;}
    void get_regdate(QString rd){registerDate = rd;}
    void get_pass(QString pass){password = pass;}
};

#endif // USUARIO_H
