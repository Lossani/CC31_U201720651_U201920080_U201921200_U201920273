#ifndef AVL_H
#define AVL_H

#include <functional>
#include <vector>
#include <QMessageBox>

using namespace std;

template <typename T, typename R = T, T NONE = nullptr>
class AVL
{
private:
    struct Node
    {
        T element;
        Node* leftChild;
        Node* rightChild;
        int height;

        Node(T element) : element(element), leftChild(nullptr), rightChild(nullptr), height(0)
        {

        }
    };

    Node* root = nullptr;
    int lenght = 0;
    function<R(T)> comparator_function;

public:
    AVL(function<R(T)> key_retriever = [] (T element) { return element; });
    ~AVL();

    int height();
    int size();
    void clear();

    void add(T element);
    //void inOrder()

    void remove(R value);
    const T find(R value);
    list<T> findAll(R value);
    list<T> findAll(R value, int limit);
    list<T> findAll(R value, bool asc, int limit);

    list<T> findAllStringsNoContains(string value, bool asc, int limit);
    list<T> findAllStringsThatContains(string value, bool asc, int limit);
    list<T> findAllStringsThatStartsWith(string value, int limit);
    list<T> findAllStringsThatEndsWith(string value, int limit);

    list<T> inOrder();
    list<T> postOrder();
    list<T> inOrder(int limit);
    list<T> postOrder(int limit);

private:
    void destroy(Node*& node);
    int height(Node* node);
    void add(Node*& node, T element);
    void remove(Node*& node, R value);

    Node*& greatest(Node*& node);
    Node*& lowest(Node*& node);
    Node* find(Node*& node, R value);

    void updateHeight(Node* node);
    void rotateLeft(Node*& node);
    void rotateRight(Node*& node);
    void balance(Node*& node);

    void findAll(list<T>& returnValues, Node*& node, R value);

    void findAllInOrder(list<T>& returnValues, Node*& node, R value, int limit);
    void findAllPostOrder(list<T>& returnValues, Node*& node, R value, int limit);

    void inOrder(list<T>& returnValues, Node*& node);
    void postOrder(list<T>& returnValues, Node*& node);
    void inOrder(list<T>& returnValues, Node*& node, int limit);
    void postOrder(list<T>& returnValues, Node*& node, int limit);

    void findAllStringsNoContainsInOrder(list<T>& returnValues, Node*& node, string value, int limit);
    void findAllStringsNoContainsPostOrder(list<T>& returnValues, Node*& node, string value, int limit);

    void findAllStringsThatContainsInOrder(list<T>& returnValues, Node*& node, string value, int limit);
    void findAllStringsThatContainsPostOrder(list<T>& returnValues, Node*& node, string value, int limit);

    void findAllStringsThatStartsWith(list<T>& returnValues, Node*& node, string value, int limit);

    void findAllStringsThatEndsWith(list<T>& returnValues, Node*& node, string value, int limit);

    Node* nullNode = nullptr;
};

template <typename T, typename R, T NONE>
AVL<T, R, NONE>::AVL(function<R(T)> key_retriever) : comparator_function(key_retriever)
{

}

template <typename T, typename R, T NONE>
AVL<T, R, NONE>::~AVL()
{
    destroy(root);
}

template <typename T, typename R, T NONE>
int AVL<T, R, NONE>::height()
{
    return height(root);
}

template <typename T, typename R, T NONE>
int AVL<T, R, NONE>::size()
{
    return lenght;
}

template <typename T, typename R, T NONE>
void AVL<T, R, NONE>::clear()
{
    destroy(root);
    lenght = 0;
}

template <typename T, typename R, T NONE>
void AVL<T, R, NONE>::add(T element)
{
    add(root, element);
}

template <typename T, typename R, T NONE>
void AVL<T, R, NONE>::remove(R value)
{
    remove(root, value);
}

template <typename T, typename R, T NONE>
const T AVL<T, R, NONE>::find(R value)
{
    Node* node = find(root, value);
    return node == nullptr ? NONE : node->element;
}

template <typename T, typename R, T NONE>
list<T> AVL<T, R, NONE>::findAll(R value)
{
    list<T> returnValues;
    findAll(returnValues, root, value);
    return returnValues;
}

template<typename T, typename R, T NONE>
list<T> AVL<T, R, NONE>::findAll(R value, int limit)
{
    list<T> returnValues;

    if (limit <= 0)
        return returnValues;

    Node* node = find(root, value);

    if (node != nullptr)
    {
        returnValues.push_back(node->element);
    }
    else
    {
        return returnValues;
    }

    if (node->rightChild != nullptr)
    {
        Node* nextNode = node;

        while (nextNode->rightChild != nullptr)
        {
            if (returnValues.size() >= limit)
                break;

            nextNode = find(nextNode->rightChild, value);

            if (nextNode != nullptr)
            {
                returnValues.push_back(nextNode->element);
            }
            else
            {
                break;
            }
        }
    }

    return returnValues;
}

template<typename T, typename R, T NONE>
list<T> AVL<T, R, NONE>::findAll(R value, bool asc, int limit)
{
    list<T> returnValues;

    if (asc)
        findAllInOrder(returnValues, root, value, limit);
    else
        findAllPostOrder(returnValues, root, value, limit);

    return returnValues;
}

template <typename T, typename R, T NONE>
list<T> AVL<T, R, NONE>::findAllStringsThatContains(string value, bool asc, int limit)
{
    //f (typeid(R) != typeid(string))
      //  return list<T>();

    list<T> returnValues;

    if (asc)
        findAllStringsThatContainsInOrder(returnValues, root, value, limit);
    else
        findAllStringsThatContainsPostOrder(returnValues, root, value, limit);

    return returnValues;
}

template <typename T, typename R, T NONE>
list<T> AVL<T, R, NONE>::findAllStringsNoContains(string value, bool asc, int limit)
{
    //if (typeid(R) != typeid(string))
      //  return list<T>();

    list<T> returnValues;

    if (asc)
        findAllStringsNoContainsInOrder(returnValues, root, value, limit);
    else
        findAllStringsNoContainsPostOrder(returnValues, root, value, limit);

    return returnValues;
}


template <typename T, typename R, T NONE>
list<T> AVL<T, R, NONE>::findAllStringsThatStartsWith(string value, int limit)
{
    if (typeid(R) != typeid(string))
        return list<T>();

    list<T> returnValues;

    findAllStringsThatStartsWith(returnValues, root, value, limit);

    return returnValues;
}

template <typename T, typename R, T NONE>
list<T> AVL<T, R, NONE>::findAllStringsThatEndsWith(string value, int limit)
{
    if (typeid(R) != typeid(string))
        return list<T>();

    list<T> returnValues;

    findAllStringsThatEndsWith(returnValues, root, value, limit);

    return returnValues;
}

template <typename T, typename R, T NONE>
list<T> AVL<T, R, NONE>::inOrder()
{
    list<T> returnValues;
    inOrder(returnValues, root);

    return returnValues;
}

template <typename T, typename R, T NONE>
list<T> AVL<T, R, NONE>::postOrder()
{
    list<T> returnValues;
    postOrder(returnValues, root);

    return returnValues;
}

template <typename T, typename R, T NONE>
list<T> AVL<T, R, NONE>::inOrder(int limit)
{
    list<T> returnValues;
    inOrder(returnValues, root, limit);

    return returnValues;
}

template <typename T, typename R, T NONE>
list<T> AVL<T, R, NONE>::postOrder(int limit)
{
    list<T> returnValues;
    postOrder(returnValues, root, limit);

    return returnValues;
}

template <typename T, typename R, T NONE>
void AVL<T, R, NONE>::destroy(Node*& node)
{
    if (node != nullptr)
    {
        destroy(node->leftChild);
        destroy(node->rightChild);
        delete node;
        node = nullptr;
    }
}

template <typename T, typename R, T NONE>
int AVL<T, R, NONE>::height(Node* node)
{
    return node == nullptr ? -1 : node->height;
}

template <typename T, typename R, T NONE>
void AVL<T, R, NONE>::add(Node*& node, T element)
{
    if (node == nullptr)
    {
        node = new Node(element);
        ++lenght;
    }
    else
    {
        if (comparator_function(element) < comparator_function(node->element))
        {
            add(node->leftChild, element);
        }
        else
        {
            add(node->rightChild, element);
        }
        balance(node);
    }
}

template <typename T, typename R, T NONE>
void AVL<T, R, NONE>::remove(Node*& node, R value)
{
    if (node == nullptr)
        return;

    if (value < comparator_function(node->element))
        remove(node->leftChild, value);
    else if (comparator_function(node->element) < value)
        remove(node->rightChild, value);
    else if (node->leftChild != nullptr && node->rightChild != nullptr)
    {
        node->element = lowest(node->rightChild)->element;
        remove(node->rightChild, comparator_function(node->element));
    }
    else
    {
        Node* oldNode = node;
        node = (node->leftChild != nullptr) ? node->leftChild : node->rightChild;
        delete oldNode;
    }

    balance(node);
}

template <typename T, typename R, T NONE>
struct AVL<T, R, NONE>::Node*& AVL<T, R, NONE>::greatest(Node*& node)
{
    return node->right == nullptr ? node : greatest(node->rightChild);
}

template <typename T, typename R, T NONE>
struct AVL<T, R, NONE>::Node*& AVL<T, R, NONE>::lowest(Node*& node)
{
    return node->leftChild == nullptr ? node : lowest(node->leftChild);
}

template <typename T, typename R, T NONE>
struct AVL<T, R, NONE>::Node* AVL<T, R, NONE>::find(Node*& node, R value)
{
    if (node == nullptr)
    {
        return nullNode;
    }
    else if (value == comparator_function(node->element))
        return node;
    else if (value < comparator_function(node->element))
        return find(node->leftChild, value);
    else
        return find(node->rightChild, value);
}

template <typename T, typename R, T NONE>
void AVL<T, R, NONE>::updateHeight(Node* node)
{
    if (node != nullptr)
    {
        node->height = max(height(node->leftChild), height(node->rightChild)) + 1;
    }
}

template <typename T, typename R, T NONE>
void AVL<T, R, NONE>::rotateLeft(Node*& node)
{
    Node* aux = node->rightChild;

    node->rightChild = aux->leftChild;
    updateHeight(node);

    aux->leftChild = node;
    updateHeight(aux);

    node = aux;
}

template <typename T, typename R, T NONE>
void AVL<T, R, NONE>::rotateRight(Node*& node)
{
    Node* aux = node->leftChild;

    node->leftChild = aux->rightChild;
    updateHeight(node);

    aux->rightChild = node;
    updateHeight(aux);

    node = aux;
}

template <typename T, typename R, T NONE>
void AVL<T, R, NONE>::balance(Node*& node)
{
    if (node == nullptr)
        return;

    int leftChildHeight = height(node->leftChild);
    int rightChildHeight = height(node->rightChild);

    if (rightChildHeight - leftChildHeight < -1)
    {
        leftChildHeight = height(node->leftChild->leftChild);
        rightChildHeight = height(node->leftChild->rightChild);

        if (rightChildHeight > leftChildHeight)
        {
            rotateLeft(node->leftChild);
        }

        rotateRight(node);
    }
    else if (rightChildHeight - leftChildHeight > 1)
    {
        leftChildHeight = height(node->rightChild->leftChild);
        rightChildHeight = height(node->rightChild->rightChild);

        if (leftChildHeight > rightChildHeight)
        {
            rotateRight(node->rightChild);
        }

        rotateLeft(node);
    }
    else
    {
        updateHeight(node);
    }
}

template<typename T, typename R, T NONE>
void AVL<T, R, NONE>::findAll(list<T> &returnValues, Node *&node, R value)
{
    Node* foundNode = find(node, value);

    if (foundNode != nullptr)
    {
        returnValues.push_back(foundNode->element);
        findAll(returnValues, foundNode->leftChild, value);
        findAll(returnValues, foundNode->rightChild, value);
    }
    else
    {
        return;
    }
}

template<typename T, typename R, T NONE>
void AVL<T, R, NONE>::findAllInOrder(list<T>& returnValues, Node*& node, R value, int limit)
{
    if (node == nullptr)
        return;

    if (returnValues.size() >= limit)
        return;

    findAllInOrder(returnValues, node->leftChild, value, limit);
    if (returnValues.size() < limit)
    {
        if (comparator_function(node->element) == value)
        {
            returnValues.push_back(node->element);
        }
    }
    else
        return;
    findAllInOrder(returnValues, node->rightChild, value, limit);
}

template<typename T, typename R, T NONE>
void AVL<T, R, NONE>::findAllPostOrder(list<T>& returnValues, Node*& node, R value, int limit)
{
    if (node == nullptr)
        return;

    if (returnValues.size() >= limit)
        return;

    findAllPostOrder(returnValues, node->rightChild, value, limit);
    if (returnValues.size() < limit)
    {
        if (comparator_function(node->element) == value)
        {
            returnValues.push_back(node->element);
        }
    }
    else
        return;
    findAllPostOrder(returnValues, node->leftChild, value, limit);
}

template <typename T, typename R, T NONE>
void AVL<T, R, NONE>::inOrder(list<T>& returnValues, Node*& node)
{
    if (node == nullptr)
        return;

    inOrder(returnValues, node->leftChild);
    returnValues.push_back(node->element);
    inOrder(returnValues, node->rightChild);
}

template <typename T, typename R, T NONE>
void AVL<T, R, NONE>::postOrder(list<T>& returnValues, Node*& node)
{
    if(node == nullptr)
        return;

    postOrder(returnValues, node->rightChild);
    returnValues.push_back(node->element);
    postOrder(returnValues, node->leftChild);
}

template <typename T, typename R, T NONE>
void AVL<T, R, NONE>::inOrder(list<T>& returnValues, Node*& node, int limit)
{
    if (node == nullptr)
        return;

    if (returnValues.size() >= limit)
        return;

    inOrder(returnValues, node->leftChild, limit);
    if (returnValues.size() < limit)
        returnValues.push_back(node->element);
    else
        return;
    inOrder(returnValues, node->rightChild, limit);
}

template <typename T, typename R, T NONE>
void AVL<T, R, NONE>::postOrder(list<T>& returnValues, Node*& node, int limit)
{
    if(node == nullptr)
        return;

    if (returnValues.size() >= limit)
        return;

    postOrder(returnValues, node->rightChild, limit);
    if (returnValues.size() < limit)
        returnValues.push_back(node->element);
    else
        return;
    postOrder(returnValues, node->leftChild, limit);
}

template <typename T, typename R, T NONE>
void AVL<T, R, NONE>::findAllStringsThatContainsInOrder(list<T>& returnValues, Node*& node, string value, int limit)
{
    if (node == nullptr)
        return;

    if (returnValues.size() >= limit)
        return;

    findAllStringsThatContainsInOrder(returnValues, node->leftChild, value, limit);

    if (returnValues.size() < limit)
    {
        if (node->element->title.find(value) != string::npos)
        {
            returnValues.push_back(node->element);
        }
    }
    else
        return;

    findAllStringsThatContainsInOrder(returnValues, node->rightChild, value, limit);
}

template <typename T, typename R, T NONE>
void AVL<T, R, NONE>::findAllStringsThatContainsPostOrder(list<T>& returnValues, Node*& node, string value, int limit)
{
    if (node == nullptr)
        return;

    if (returnValues.size() >= limit)
        return;

    findAllStringsThatContainsPostOrder(returnValues, node->rightChild, value, limit);

    if (returnValues.size() < limit)
    {
        if (node->element->title.find(value) != string::npos)
        {
            returnValues.push_back(node->element);
        }
    }
    else
        return;

    findAllStringsThatContainsPostOrder(returnValues, node->leftChild, value, limit);
}

template <typename T, typename R, T NONE>
void AVL<T, R, NONE>::findAllStringsNoContainsInOrder(list<T>& returnValues, Node*& node, string value, int limit)
{
    if (node == nullptr)
        return;

    if (returnValues.size() >= limit)
        return;

    findAllStringsNoContainsInOrder(returnValues, node->leftChild, value, limit);

    if (returnValues.size() < limit)
    {
        if (node->element->title.find(value) == string::npos)
        {
            returnValues.push_back(node->element);
        }
    }
    else
        return;

    findAllStringsNoContainsInOrder(returnValues, node->rightChild, value, limit);
}

template <typename T, typename R, T NONE>
void AVL<T, R, NONE>::findAllStringsNoContainsPostOrder(list<T>& returnValues, Node*& node, string value, int limit)
{
    if (node == nullptr)
        return;

    if (returnValues.size() >= limit)
        return;

    findAllStringsNoContainsPostOrder(returnValues, node->rightChild, value, limit);

    if (returnValues.size() < limit)
    {
        if (node->element->title.find(value) == string::npos)
        {
            returnValues.push_back(node->element);
        }
    }
    else
        return;

    findAllStringsNoContainsPostOrder(returnValues, node->leftChild, value, limit);
}

template<typename T, typename R, T NONE>
void AVL<T, R, NONE>::findAllStringsThatStartsWith(list<T> &returnValues, Node*& node, string value, int limit)
{
    if (returnValues.size() >= limit)
        return;

    if (node == nullptr)
        return;

    if (value.size() <= comparator_function(node->element).size())
    {
        string startString = comparator_function(node->element).substr(0, value.size());
        if (value == startString)
        {
            returnValues.push_back(node->element);
            findAllStringsThatStartsWith(returnValues, node->leftChild, value, limit);
            findAllStringsThatStartsWith(returnValues, node->rightChild, value, limit);
        }
        else if (value < startString)
            findAllStringsThatStartsWith(returnValues, node->leftChild, value, limit);
        else
            findAllStringsThatStartsWith(returnValues, node->rightChild, value, limit);
    }
    else
    {
        //findAllStringsThatStartsWith(returnValues, node->leftChild, value, limit);
        findAllStringsThatStartsWith(returnValues, node->rightChild, value, limit);
    }
}

template<typename T, typename R, T NONE>
void AVL<T, R, NONE>::findAllStringsThatEndsWith(list<T> &returnValues, Node*& node, string value, int limit)
{
    if (returnValues.size() >= limit)
        return;

    if (node == nullptr)
        return;

    if (value.size() <= comparator_function(node->element).size())
    {
        string endString = comparator_function(node->element).substr(comparator_function(node->element).size() - value.size(), value.size());
        if (value == endString)
        {
            returnValues.push_back(node->element);
        }
    }

    findAllStringsThatEndsWith(returnValues, node->leftChild, value, limit);
    findAllStringsThatEndsWith(returnValues, node->rightChild, value, limit);
}

#endif // AVL_H
