/*
       **********************************************
       *    Clase genérica que implementa listas    *
       **********************************************
*/

#ifndef LISTMANAGER_H
#define LISTMANAGER_H

#include <list>
#include <stack>
#include <iostream>
#include <functional>
#include <fstream>
#include <iterator>
#include <QMessageBox>
using namespace std;

template<typename T, typename R, typename S>
class ListController
{
protected:
    list<T> all_elements;

private:
    function<R(T)> comparator;
    function<bool(S, S)> comparator_function;
    function<void(ofstream& file, T element)> save_function;
    function<list<T>(ifstream& file)> retrieve_function;
    //function<list<string>(T element, list<int>& final_size)> parser;
    //function<list<T>(string data, int data_size)> interpreter;
    string filename;
public:
    ListController(function<R(T)> comparator, function<bool(S, S)> comparator_function, function<void(ofstream& file, T element)> save_function, function<list<T>(ifstream& file)> retrieve_function, string filename);
    ~ListController();

    virtual void save_elements();
    virtual void retrieve_elements();
    virtual void add_element(T element);
    virtual void update_element(T updated_element);
    virtual void delete_element(R identificator);

    virtual list<T> get_all_elements();
    virtual T get_element(R identificator);
    virtual T get_element(function<S(T)> field_getter, S field_to_compare);
    virtual list<T> find_elements(function<S(T)> field_getter, S field_to_compare);


};

template<typename T, typename R, typename S>
ListController<T, R, S>::ListController(function<R(T)> comparator, function<bool(S, S)> comparator_function, function<void(ofstream& file, T element)> save_function, function<list<T>(ifstream& file)> retrieve_function, string filename) : comparator(comparator), comparator_function(comparator_function), save_function(save_function), retrieve_function(retrieve_function), filename(filename)
{
    retrieve_elements();
}

template<typename T, typename R, typename S>
ListController<T, R, S>::~ListController()
{
    all_elements.clear();
}

template<typename T, typename R, typename S>
void ListController<T, R, S>::save_elements()
{
    ofstream file;

    file.open(filename, ios::out);

    if (file.is_open())
    {
        for (T element : all_elements)
        {
            save_function(file, element);
            //file.write((const char*) &element, sizeof(T));
        }

        file.close();
    }
}
template<typename T, typename R, typename S>
void ListController<T, R, S>::retrieve_elements()
{
    ifstream file;

    file.open(filename, ios::in);

    //file.seekg(0, ios::end);
    //int num_elements = file.tellg() / sizeof(T);

    if (file.is_open())
    {
        all_elements = retrieve_function(file);

        file.close();
        //for (int i = 0; i < num_elements; ++i)
        //{
        //    T current_element;

        //   file.seekg(i * sizeof(T), ios::beg);

        //   file.read((char*) &current_element, sizeof(T));

        //   all_elements.push_back(current_element);
        //}
    }
}
template<typename T, typename R, typename S>
void ListController<T, R, S>::add_element(T element)
{
    all_elements.push_back(element);
    save_elements();
}

template<typename T, typename R, typename S>
void ListController<T, R, S>::update_element(T updated_element)
{
    list<T> new_list;

    bool was_updated = false;
    R identificator = comparator(updated_element);
    for (T element : all_elements)
    {
        if (identificator == comparator(element))
        {
            new_list.push_back(updated_element);
            was_updated = true;
        }
        else
        {
            new_list.push_back(element);
        }
    }

    if (was_updated)
    {
        all_elements = new_list;
        save_elements();
    }
}

template<typename T, typename R, typename S>
void ListController<T, R, S>::delete_element(R identificator)
{
    list<T> new_list;
    bool was_deleted = false;

    for (T element: all_elements)
    {
        if (identificator == comparator(element))
        {
            was_deleted = true;
        }
        else
        {
            new_list.push_back(element);
        }
    }

    all_elements = new_list;

    if (was_deleted)
        save_elements();
}

template<typename T, typename R, typename S>
list<T> ListController<T, R, S>::get_all_elements()
{
    return all_elements;
}

template<typename T, typename R, typename S>
T ListController<T, R, S>::get_element(R identificator)
{
    for (T element : all_elements)
    {
        if (identificator == comparator(element))
        {
            return element;
        }
    }

    return T();
}

template<typename T, typename R, typename S>
T ListController<T, R, S>::get_element(function<S(T)> field_getter, S field_to_compare)
{
    for (T element : all_elements)
    {
        //QMessageBox msg;
        //msg.setText("field_getter(element)");
        //msg.exec();
        if (comparator_function(field_getter(element), field_to_compare))
        {
            return element;
        }
    }

    return T();
}

template<typename T, typename R, typename S>
list<T> ListController<T, R, S>::find_elements(function<S(T)> field_getter, S field_to_compare)
{
    list<T> return_list;

    for (T element : all_elements)
    {
        if (comparator_function(field_getter(element), field_to_compare))
        {
            return_list.push_back(element);
        }
    }

    return return_list;
}


#endif // LISTMANAGER_H