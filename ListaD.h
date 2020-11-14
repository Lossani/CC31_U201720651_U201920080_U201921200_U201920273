#ifndef LISTAD_H
#define LISTAD_H

template <typename T>
class Nodo {
public:
    T data;
    Nodo* next;
    Nodo* ant;
    Nodo(T data, Nodo* next=nullptr, Nodo* ant=nullptr): data(data), next(next),ant(ant){}
};

template <typename T>
class ListaD{
public:
    int len = 0;

    Nodo<T>* head = nullptr;

    Nodo<T>* selected = nullptr;

    int get_len(){return len;}

    void append(T data){

        len++;

      Nodo<T>* nodo = new Nodo<T>(data);

      if(this->head==nullptr)
      {
        this->head = nodo;
        this->selected = head;
        return;
      }

      Nodo<T>* last = this->head;
      while (last->next!=nullptr)
      {
        last = last->next;
      }

      last->next = nodo;
      nodo->ant = last;

    }

    void eliminar(){

      Nodo<T>* temp = this->selected;
      //verificar si está vacio
      if(temp == nullptr) return;

      //verificar si es el unico nodo
      if(temp->ant == nullptr and temp->next == nullptr){
          this->head = nullptr;
          selected = head;
          free(temp);
          return;
      }

      if(temp->ant!=nullptr){
        temp->ant->next = temp->next;
        this->selected = temp->ant;
      }

      if(temp->next!=nullptr){
        temp->next->ant = temp->ant;
        this->selected = temp->next;
        free(temp);
      }

    }

    int getCount()  {
        int count = 0;
        Nodo<T>* current = this->head;
        while (current != nullptr)
        {
            count++;
            current = current->next;
        }
        return count;
    }

    void sgt(){
      if(this->selected->next!=nullptr)
      {
        selected = selected->next;
      }
    }

    void ant(){
      if(this->selected->ant!=nullptr)
      {
        selected = selected->ant;
      }
    }

    bool EsVacio(){
      return this->head == nullptr;
    }
};


#endif // LISTAD_H
