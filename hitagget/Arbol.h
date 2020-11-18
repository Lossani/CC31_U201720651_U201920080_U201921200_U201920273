#ifndef ARBOL_H
#define ARBOL_H

#include <iostream>
#include <vector>
#include <time.h>
#include <functional>
#include <QTextStream>
#include <QListWidget>


using namespace std;

template <typename T>
class Node{
  public:
  T data;
  Node* left;
  Node* right;
  Node(T data, Node* left = nullptr, Node* right = nullptr): data(data), left(left), right(right){}
};

template <typename OBJ,typename KEY = OBJ>
class BST{
  public:
  Node<OBJ>* root;
  function<KEY(OBJ)> key;

  BST(function<KEY(OBJ)> key = [](OBJ obj){return obj;},Node<OBJ>* root = nullptr):key(key),root(root){}

  void add(OBJ val){add(this->root, val);}
  void add(Node<OBJ>*& node, OBJ val){
    Node<OBJ>* new_node = new Node<OBJ>(val);
    if(node == nullptr){
      node = new_node;
      return;
    }
    if(key(val) <= key(node->data)){
      add(node->left, val);
    }else{ //VAL > NODE->DATA
      add(node->right, val);
    }
  }
  void del(OBJ val){del(this->root, val);}
  void del(Node<OBJ>*& node, OBJ val){
    //CASO BASE
    if(node == nullptr) return;

    if(key(val) < key(node->data)){
      del(node->left, val);
    } else if(key(val) > key(node->data)){
      del(node->right, val);
    }else{
      if(node->left == nullptr){//NO TEIENE HIJO IZQUIERDO
        node = node->right;
      } else if(node->right == nullptr){//NO TIENE HIJO DERECHO
        node = node->left;
      }else{
        //TODO: MIN_NODE
        Node<OBJ>* temp = min_node(node->right);
        node->data = temp->data;
        del(node->right, temp->data);
      }
    }
  }

  int clim;

  void inorder(QListWidget* lista, int lim){ clim = 0; lim=10; inorder(lista, this->root, lim);}
  void inorder(QListWidget* lista, Node<OBJ>* node, int lim){
    //CASO BASE

    if(node == nullptr) return;

    inorder(lista,node->left,lim);

    if(clim<lim){
        lista->addItem(node->data.toString());
    }
            clim++;
        inorder(lista,node->right,lim);
  }

  void postorder(QListWidget* lista, int lim){ clim = 0; postorder(lista, this->root, lim);}
  void postorder(QListWidget* lista, Node<OBJ>* node,int lim){
    //CASO BASE

    if(node == nullptr) return;
        postorder(lista,node->right,lim);

    if(clim<lim){
        lista->addItem(node->data.toString());
    }
            clim++;
            postorder(lista,node->left,lim);

  }
  // %%% %%%
  //Lambda [](Juego j){return j.toString();}
  void inorder_l(function<void(OBJ)> imprimir) { inorder_l(imprimir, this->root);}
  void inorder_l(function<void(OBJ)> imprimir, Node<OBJ>* node){
    if(node == nullptr) return;
    inorder_l(imprimir,node->left);
    imprimir(node->data);
    inorder_l(imprimir,node->right);
  }

  Node<OBJ>* min_node(Node<OBJ>* node){
    if(node->left == nullptr) return node;
    while(node && node->left != nullptr){
      node = node->left;
    }
    return node;
  }

};

#endif // ARBOL_H
