#include <iostream>
#include "usermanager.h"
#include <fstream>

#include <QMessageBox>
using namespace std;

UserManager::UserManager() : FollowerManager(), ListController<User*, int, string>(
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

            getline(file, currentUser->email, '\t');
            getline(file, currentUser->fullname, '\t');
            getline(file, currentUser->registerDate);

            currentUser->id = stoi(id);

            currentUser->password = currentUser->email; // Using same email as password temporarily

            currentUser->followedUsers = getUserFollowedUsersIds(currentUser->id);

            //QMessageBox msg;
            //msg.setText(to_string(currentUser->email.size()).c_str());
            //msg.exec();
            retrievedElements.push_back(currentUser);

            avl_users_by_email->add(currentUser);
        }

        return retrievedElements;
    },
    "users.tsv")
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
    if (ListController<User*, int, string>::find_elements(email_field_comparator, email).size() > 0)
    {
        return false;
    }

    User* newUser = new User();

    newUser->id = time(0);
    newUser->email = email;
    newUser->password = password;
    newUser->fullname = fullname;

    ListController<User*, int, string>::add_element(newUser);
    return true;
}

void UserManager::updateUser(User* user)
{
    ListController<User*, int, string>::update_element(user);
}

User* UserManager:: getUserByEmail(string email)
{
    return avl_users_by_email->find(email);
}

User* UserManager:: getUserById(int id)
{
    return ListController<User*, int, string>::get_element(id);
}
