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
    [this](ifstream& file)
    {
        avl_users_by_email = new AVL<User*, string, nullptr>([](User* element) { return element->email; });

        list<User*> retrievedElements;

        User* currentUser;

        string id;

        getline(file, id); // Ignoring first line, temporarily using id for getline parameter

        while (getline(file, id, '\t'))
        {
            currentUser = new User();

            //getline(file, id, ','); // Retrieving the real ID of the user
            getline(file, currentUser->email, '\t');
            getline(file, currentUser->fullname, '\t');
            //getline(file, currentUser->registerDate, ',');
            getline(file, currentUser->registerDate);
            //getline(file, currentUser->password); // We have no password yet

            currentUser->id = stoi(id);
/*
            // Data in files are between double quote marks, we must trim them
            currentUser->email = currentUser->email.erase(currentUser->email.find_last_not_of("\" ") + 1);
            currentUser->email = currentUser->email.erase(0, currentUser->email.find_first_not_of("\" "));
            currentUser->fullname = currentUser->fullname.erase(currentUser->fullname.find_last_not_of("\" ") + 1);
            currentUser->fullname = currentUser->fullname.erase(0, currentUser->fullname.find_first_not_of("\" "));
*/

            currentUser->password = currentUser->email; // Using same email as password temporarily

            //QMessageBox msg;
            //msg.setText(to_string(currentUser->email.size()).c_str());
            //msg.exec();
            retrievedElements.push_back(currentUser);

            avl_users_by_email->add(currentUser);
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
    "users.tsv")
{
    email_field_comparator = [](User* user)
    {
        return user->email;
    };

    for (User* user : get_all_elements())
    {
        this->avl_users_by_email->add(user);
    }
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

const User* UserManager:: getUserByEmail(string email)
{
    return avl_users_by_email->find(email);
    //return get_element(email_field_comparator, email);
}

const User* UserManager:: getUserById(int id)
{
    return get_element(id);
}
