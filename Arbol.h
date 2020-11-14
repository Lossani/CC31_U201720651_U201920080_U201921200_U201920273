#ifndef ARBOL_H
#define ARBOL_H


#include <functional>
#include <string>
#include <fstream>

using namespace std;

template <typename T, typename R = T>
class Bst {
    struct Node {
        T       elem;
        Node* left;
        Node* right;

        Node(T elem, Node* left = nullptr, Node* right = nullptr)
            : elem(elem), left(left), right(right) {}
    };

    using lmbKey = function<R(T)>;
    using lmbP = function<void(T)>;

    Node* root;
    int     len;
    lmbKey  key;
public:
    Bst(lmbKey key = [](T a) {return a; }) : root(nullptr), len(0), key(key) {}
    ~Bst() { destroy(root); }

    void add(T elem) { add(root, elem); }
    void inOrder(lmbP proc) { inOrder(root, proc); }
    void preOrder(lmbP proc) { preOrder(root, proc); }
    void postOrder(lmbP proc) { postOrder(root, proc); }

    T find(R attr) {
        auto node = find(root, attr);
        return node == nullptr ? nullptr : node->elem;
    }
    T greatest() {
        if (root == nullptr) {
            return nullptr;
        }
        else {
            auto node = greatest(root);
            return node->elem;
        }
    }
    void remove(R attr) {
        Node*& node = find(root, attr); // NO USAR AUTO par atipos referencia
        if (node == nullptr) return;
        if (node->left == nullptr) {
            Node* aux = node;
            node = node->right;
            delete aux;
        }
        else {
            Node*& cand = greatest(node->left);
            node->elem = cand->elem;
            Node* aux = cand;
            cand = cand->left;
            delete aux;
        }
    }

    //-------------------------------------------------------------------
    // CÓDIGO SUPER OPCIONAL (NO ES NECESARIO ENTENDER NI USAR SI NO DESEA)
    //-------------------------------------------------------------------
    void generateDot(std::string filename) {
        if (root != nullptr) {
            std::ofstream file(filename);
            file << "digraph G {\n";
            generate(root, file);
            file << "}";
        }
    }

    void generate(Node* node, std::ofstream& file) {
        if (node->left != nullptr) {
            file << node->elem << "->" << node->left->elem << std::endl;
            generate(node->left, file);
        }
        else {
            file << "l" << node->elem << " [shape=point]\n";
            file << node->elem << "->l" << node->elem << std::endl;
        }
        if (node->right != nullptr) {
            file << node->elem << "->" << node->right->elem << std::endl;
            generate(node->right, file);
        }
        else {
            file << "r" << node->elem << " [shape=point]\n";
            file << node->elem << "->r" << node->elem << std::endl;
        }
    }

    void elim(int n) {
        elimigual(n);
    }

    void elimigual() {
        elimiguales(root);
    }

    //-------------------------------------------------------------------
    // (end) CÓDIGO SUPER OPCIONAL (NO ES NECESARIO ENTENDER NI USAR SI NO DESEA)
    //-------------------------------------------------------------------

private:

    void buscar2(Node*& node, R attr, Node* cont) {

        if (node != nullptr) {
            if (attr == key(node->elem)) {
                if (&node != &cont) {
                        if (node->left == nullptr) {
                            Node* aux = node;
                            node = node->right;
                            delete aux;
                        }
                        else {
                            Node*& cand = greatest(node->left);
                            node->elem = cand->elem;
                            Node* aux = cand;
                            cand = cand->left;
                            delete aux;
                        }
                }

            }
            //buscar2(node->left, attr, cont);
            buscar2(node->right, attr, cont);
        }
        else return;
    }

    void elimigual(int buscar) {

        Node*& noden = find(root, buscar);

        while (find(noden->right,noden->elem) != nullptr) {

            Node*& node = find(noden->right, buscar); // NO USAR AUTO para tipos referencia
            if (node == nullptr) return;
            if (node->left == nullptr) {
                Node* aux = node;
                node = node->right;
                delete aux;
            }
            else {
                Node*& cand = greatest(node->left);
                node->elem = cand->elem;
                Node* aux = cand;
                cand = cand->left;
                delete aux;
            }

            //int cont = 0;
            //int busc = node->elem;
            //buscar2(node, busc, node);
            ////elimigual(node->left);
            ////elimigual(node->right);
        }
    }

    void elimiguales(Node* test) {

        elimigual(test->elem);

        if(test->left != nullptr)
        elimiguales(test->left);
        if (test->right != nullptr)
        elimiguales(test->right);

    }






    void destroy(Node* node) {
        if (node != nullptr) {
            destroy(node->left);
            destroy(node->right);
            delete node;
        }
    }
    void add(Node*& node, T elem) {
        if (node == nullptr) {
            node = new Node(elem);
            ++len;
        }
        else if (key(elem) < key(node->elem)) {
            add(node->left, elem);
        }
        else if (key(elem) >= key(node->elem)) {
            add(node->right, elem);
        }
    }
    void inOrder(Node* node, lmbP proc) {
        if (node != nullptr) {
            inOrder(node->left, proc);
            proc(node->elem);
            inOrder(node->right, proc);
        }
    }
    void postOrder(Node* node, lmbP proc) {
        if (node != nullptr) {
            postOrder(node->left, proc);
            postOrder(node->right, proc);
            proc(node->elem);
        }
    }
    void preOrder(Node* node, lmbP proc) {
        if (node != nullptr) {
            proc(node->elem);
            preOrder(node->left, proc);
            preOrder(node->right, proc);
        }
    }
    Node*& find(Node*& node, R attr) {
        if (node == nullptr) {
            return dummynull;
        }
        else if (attr == key(node->elem)) {
            return node;
        }
        else if (attr < key(node->elem)) {
            return find(node->left, attr);
        }
        else {
            return find(node->right, attr);
        }
        // return find((attr < key(node->elem)? node->left: node->right, attr);
    }

    Node*& greatest(Node*& node) {
        return node->right == nullptr ? node : greatest(node->right);
    }

    // Mandrakeada
    Node* dummynull = nullptr;



};


#endif // ARBOL_H
