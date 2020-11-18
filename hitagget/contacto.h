#ifndef CONTACTO_H
#define CONTACTO_H
#include <QApplication>
#include <string>
using namespace std;
class Contacto{
private:
    QString numero;
    QString comp_name;
    QString apodo;
public:

    Contacto(){      
        numero = "";
        comp_name = "";
        apodo = "";
    }
    Contacto(QString n, QString num, QString ap){

        numero = num;
        comp_name = n;
        apodo = ap;
    }

    ~Contacto(){
    }

    QString get_numero(){return numero;}
    QString get_comp_name(){return comp_name;}
    QString get_apodo(){return apodo;}
    void set_numero(QString n){numero = n;}
    void set_comp_name(QString e){comp_name = e;}
    void set_apodo(QString es){apodo = es;}

    QString fill_space(QString fill, int n = 15){
        int sep = n-fill.size();
        if(sep % 2 != 0)
            return QString((sep/2),' ') + fill + QString((sep/2)+1,' ');
        return QString((sep/2),' ') + fill + QString((sep/2),' ');
    }

    QString toString(){
       return fill_space(this->comp_name,30)+" | "+fill_space(this->numero,15)+" | "+fill_space(this->apodo);
    }

};

#endif // CONTACTO_H
