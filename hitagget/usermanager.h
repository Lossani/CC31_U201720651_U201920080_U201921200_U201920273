#ifndef USERMANAGER_H
#define USERMANAGER_H
#include <iostream>
#include "listcontroller.h"
#include "avl.h"
#include <vector>
#include "user.h"

using namespace std;

class UserManager : private ListController<User*, int, string>
{
private:
    function<string(User*)> email_field_comparator;
    AVL<User*, string, nullptr>* avl_users_by_email;

public:
    UserManager();
    ~UserManager();

    bool addUser(string email, string fullname, string password);
    void updateUser(User* user);
    const User* getUserByEmail(string email);
    const User* getUserById(int id);
    //void delete_user(int id);
};

#endif // USERMANAGER_H
