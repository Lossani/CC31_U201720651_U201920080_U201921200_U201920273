#include <iostream>
#include "usermanager.h"
#include <fstream>

#include <QMessageBox>
using namespace std;

UserManager::UserManager() : ListController<User, int, string>(
    [](User user)
    {
        return user.id;
    },
    [](string value1, string value2)
    {
        return value1 == value2;
    },
    [](ofstream& file, User element)
    {
        file << element.id << endl;
        file << element.email << endl;
        file << element.fullname << endl;
        file << element.password << endl;
        file << element.registerDate << endl;
        file << element.is_admin << endl;
    },
    [](ifstream& file)
    {
        list<User> retrieved_elements;

        while(!file.eof())
        {
            User current_user;

            file >> current_user.id >> current_user.email >> current_user.fullname >> current_user.password >> current_user.registerDate >> current_user.is_admin;

            retrieved_elements.push_back(current_user);
        }

        return retrieved_elements;
    },
    "users.dat")
{
    email_field_comparator = [](User user)
    {
        return user.email;
    };
}

UserManager::~UserManager()
{

}

bool UserManager::add_user(string email, string fullname, string password)
{
    if (find_elements(email_field_comparator, email).size() > 0)
    {
        return false;
    }

    User new_user;

    new_user.id = time(0);
    new_user.email = email;
    new_user.password = password;
    new_user.fullname = fullname;

    if (email == "admin")
        new_user.is_admin = true;

    add_element(new_user);
    return true;
}

void UserManager::update_user(User user_to_update)
{
    update_element(user_to_update);
}

User UserManager:: get_user_by_email(string email)
{
    return get_element(email_field_comparator, email);
}

User UserManager:: get_user_by_id(int id)
{
    return get_element(id);
}
