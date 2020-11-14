#ifndef USER_H
#define USER_H
#include <iostream>

using namespace std;

class User
{
public:
    int id = -1;
    string email = "none";
    string password = "none";
    string fullname = "none";
    int registerDate = 0;
    bool is_admin = false;
};

#endif // USER_H
