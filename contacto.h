#ifndef CONTACTO_H
#define CONTACTO_H
#include <QApplication>
#include <string>
using namespace std;
class Contacto{
private:
    QString numero;
    QString nombre;
    QString apellido;
    QString apodo;
public:

    Contacto(){
        apellido = "";
        numero = "";
        nombre = "";
        apodo = "";
    }
    Contacto(QString n, QString a, QString num, QString ap){
        apellido = a;
        numero = num;
        nombre = n;
        apodo = ap;
    }

    ~Contacto(){
    }

    QString get_numero(){return numero;}
    QString get_nombre(){return nombre;}
    QString get_apellido(){return apellido;}
    QString get_apodo(){return apodo;}
    void set_apellido(QString ap){apellido = ap;}
    void set_numero(QString n){numero = n;}
    void set_nombre(QString e){nombre = e;}
    void set_apodo(QString es){apodo = es;}
};

#endif // CONTACTO_H
