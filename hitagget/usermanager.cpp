#include <iostream>
#include "usermanager.h"
#include <fstream>

#include <QMessageBox>
#include <QTime>
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
    [this]()
    {
         ofstream file;

         file.open("users.tsv", ios::out);

         if (file.is_open())
         {
             file << "id	email	name	joindate" << endl;

             for (User* user : ListController<User*, int, string>::get_all_elements())
             {
                 file << user->id << '\t'
                      << user->email << '\t'
                      << user->fullname << '\t'
                      << user->registerDate
                      << endl;
             }

             file.close();
         }
    },
    [this](ifstream& file)
    {
        avl_users_by_email = new AVL<User*, string, nullptr>([](User* element) { return element->email; });
        avl_users_by_id = new AVL<User*, int, nullptr>([](User* element) { return element->id; });

        currentIndex = 1;

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

            currentUser->followedUsers = getUserFollowedUsers(currentUser->id);

            currentUser->followedUsers.unique([](Follower* follower1, Follower* follower2) { return follower1->followedUserID == follower2->followedUserID; });

            if (currentUser->id > currentIndex)
                currentIndex = currentUser->id;

            retrievedElements.push_back(currentUser);

            avl_users_by_email->add(currentUser);
            avl_users_by_id->add(currentUser);
        }

        return retrievedElements;
    },
    "users.tsv")
{
    /*if (avl_users_by_email == nullptr)
        avl_users_by_email = new AVL<User*, string, nullptr>([](User* element) { return element->email; });*/
}

UserManager::~UserManager()
{

}

bool UserManager::addUser(string email, string fullname, string password)
{
    if (avl_users_by_email->find(email))
    {
        return false;
    }

    User* newUser = new User();

    newUser->id = ++currentIndex;
    newUser->email = email;
    newUser->password = password;
    newUser->fullname = fullname;

    newUser->registerDate = QDateTime::currentDateTime().toString("yyyy-MM-dd").toStdString();

    ListController<User*, int, string>::add_element(newUser);

    avl_users_by_email->add(newUser);

    return true;
}

User* UserManager:: getUserByEmail(string email)
{
    return avl_users_by_email->find(email);
}

User* UserManager:: getUserById(int id)
{
    return avl_users_by_id->find(id);
}

void UserManager::saveUsers()
{
    ListController<User*, int, string>::save_elements();
}

int UserManager::getUsersCount()
{
    return ListController<User*, int, string>::all_elements.size();
}
