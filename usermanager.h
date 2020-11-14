#ifndef USERMANAGER_H
#define USERMANAGER_H
#include <iostream>
#include "listcontroller.h"
#include <vector>
#include "user.h"

using namespace std;

class UserManager : private ListController<User, int, string>
{
private:
    function<string(User)> email_field_comparator;
public:
    UserManager();
    ~UserManager();

    bool add_user(string email, string fullname, string password);
    void update_user(User user_to_update);
    User get_user_by_email(string email);
    User get_user_by_id(int id);
    //void delete_user(int id);
};

#endif // USERMANAGER_H
