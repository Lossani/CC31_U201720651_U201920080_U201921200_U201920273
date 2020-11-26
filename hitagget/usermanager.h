#ifndef USERMANAGER_H
#define USERMANAGER_H
#include <iostream>
#include "listcontroller.h"
#include "followermanager.h"
#include "avl.h"
#include <vector>
#include "user.h"

using namespace std;

class UserManager : public FollowerManager, private ListController<User*, int, string>
{
private:
    AVL<User*, string, nullptr>* avl_users_by_email;

    int currentIndex;

public:
    UserManager();
    ~UserManager();

    bool addUser(string email, string fullname, string password);
    User* getUserByEmail(string email);
    User* getUserById(int id);

    void saveUsers();

    int getUsersCount();
};

#endif // USERMANAGER_H
