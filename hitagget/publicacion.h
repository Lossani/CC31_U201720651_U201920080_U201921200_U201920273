#ifndef PUBLICACION_H
#define PUBLICACION_H

#include <QApplication>

class Hitagged {
private:

    struct Comment{
        QString date;
        QString text;
    };

    struct Interacton{
        QString fecha;
        //Shared
    };

  QString nombre;
  QString hashtag;
  QString titulo;
  QString texto;
  int num_likes;
  int num_interactions;


public:

  Hitagged(){
    titulo = "";
    texto = "";
  }

  Hitagged(QString name, QString titulo, QString texto, QString ht){
      this->nombre = name;
    this->titulo = titulo;
    this->texto = texto;
      this->hashtag = ht;
  }

  ~Hitagged(){}

  QString get_nombre(){return nombre;}
  QString get_titulo(){return titulo;}
  QString get_texto(){return texto;}
  QString get_hashtag(){return hashtag;}

};
#endif // PUBLICACION_H
