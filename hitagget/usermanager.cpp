#include <iostream>
#include "usermanager.h"
#include <fstream>

#include <QMessageBox>
using namespace std;

UserManager::UserManager() : ListController<User*, int, string>(
    [](User* user)
    {
        return user->id;
    },
    [](string value1, string value2)
    {
        return value1 == value2;
    },
    [](ofstream& file, User* element)
    {
        file << element->id << endl;
        file << element->email << endl;
        file << element->fullname << endl;
        file << element->registerDate << endl;
        file << element->password;
    },
    [](ifstream& file)
    {
        list<User*> retrievedElements;

        User* currentUser;

        string id;

        while (getline(file, id, ','))
        {
            currentUser = new User();

            getline(file, currentUser->email, ',');
            getline(file, currentUser->fullname, ',');
            getline(file, currentUser->registerDate, ',');
            getline(file, currentUser->password);

            currentUser->id = stoi(id);

            retrievedElements.push_back(currentUser);
        }

        return retrievedElements;
        /*
        while(!file.eof())
        {
            User current_user;

            file >> current_user.id >> current_user.email >> current_user.fullname >> current_user.password >> current_user.registerDate >> current_user.is_admin;

            retrievedElements.push_back(current_user);
        }

        return retrieved_elements;
        */
    },
    "users.dat")
{
    email_field_comparator = [](User* user)
    {
        return user->email;
    };
}

UserManager::~UserManager()
{

}

bool UserManager::addUser(string email, string fullname, string password)
{
    if (find_elements(email_field_comparator, email).size() > 0)
    {
        return false;
    }

    User* newUser = new User();

    newUser->id = time(0);
    newUser->email = email;
    newUser->password = password;
    newUser->fullname = fullname;

    add_element(newUser);
    return true;
}

void UserManager::updateUser(User* user)
{
    update_element(user);
}

User* UserManager:: getUserByEmail(string email)
{
    return get_element(email_field_comparator, email);
}

User* UserManager:: getUserById(int id)
{
    return get_element(id);
}
