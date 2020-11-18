#ifndef LISTAT_H
#define LISTAT_H

#include <QApplication>
#include <iostream>

using namespace std;

class ListaT {
    struct Node {
        QString elem;
        Node* next;

        Node(QString elem, Node* next=nullptr): elem(elem), next(next) {}
    };

    Node*   head;
    int     len;
public:
    ListaT(): head(nullptr), len(0) {}


    ~ListaT() {
        while (head != nullptr) {
            Node* aux = head;
            head = head->next;
            delete aux;
        }
    }

    int size () {
        return len;
    }
    bool empty () {
        return len == 0;
    }



    void addFirst (QString elem) {
        head = new Node(elem, head);
        ++len;
    }
    void addPos (QString elem, int pos) {
        if (pos < 0 || pos > len) return;
        if (pos == 0) {
            addFirst(elem);
        } else {
            Node* aux = head;
            for (int i = 1; i < pos; ++i) {
                aux = aux->next;
            }
            aux->next = new Node(elem, aux->next);
            ++len;
        }
    }
    void addLast (QString elem) {
        addPos(elem, len);
    }

    void removeFirst () {
        if (len > 0) {
            Node* aux = head;
            head = head->next;
            delete aux;
            --len;
        }
    }

    void removePos (int pos) {
        if (pos < 0 || pos >= len) return;
        if (pos == 0) {
            removeFirst();
        }
        else {
            Node* aux = head;
            for (int i = 1; i < pos; ++i) {
                aux = aux->next;
            }
            Node* aux2 = aux->next;
            aux->next = aux2->next;
            delete aux2;
            --len;
        }
    }
    void removeLast () {
        removePos(len-1);
    }

    void editFirst (QString elem) {
        if (len > 0) {
            head->elem = elem;
        }
    }
    void editPos (QString elem, int pos) {
        if (pos < 0 || pos >= len) return;
        Node* aux = head;
        for (int i = 0; i < pos; ++i) {
            aux = aux->next;
        }
        aux->elem = elem;
    }
    void editLast (QString elem) {
        editPos(elem, len-1);
    }

    QString getFirst () {
        if (len > 0) {
            return head->elem;
        } else {
            return 0;
        }
    }

    QString get (int pos) {
        if (pos < 0 || pos >= len) return nullptr; // ERROR
        Node* aux = head;
        for (int i = 0; i < pos; ++i) {
            aux = aux->next;
        }
        return aux->elem;
    }

    QString getLast () {
        return get(len-1);
    }

    int find (QString elem) {
        Node* aux = head;
        for (int i = 0; i < len; ++i) {
            if (aux->elem == elem) {
                return i;
            }
            aux = aux->next;
        }
        return -1;
    }
    int *findAll (QString elem) {
        int tam = 0;
        int *c = new int[tam];

        Node* aux = head;
        for (int i = 0; i < len; ++i) {
            if (aux->elem == elem) {
                tam++;
                int *temp = new int[tam];
                temp[tam-1]=i;
                c=temp;
            }
            aux = aux->next;
        }
        return c;
    }


};
#endif // LISTAT_H
