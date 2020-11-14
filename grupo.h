#ifndef GRUPO_H
#define GRUPO_H
#include <QApplication>
class Grupo{
private:
  QString nombre;
  QString n_integrantes;
  QString Describcion;
public:
  Grupo(){
    nombre = "";
    n_integrantes = "";
    Describcion = "";
  }

  Grupo(QString nom, QString n, QString Describcion){
    this->nombre = nom;
    this->n_integrantes = n;
    this->Describcion = Describcion;
  }

  ~Grupo(){}

  QString get_nombre(){return nombre;}
  QString get_n(){return n_integrantes;}
  QString get_Describcion(){return Describcion;}
};
#endif // GRUPO_H
