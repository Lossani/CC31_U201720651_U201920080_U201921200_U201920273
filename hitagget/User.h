#ifndef USER_H
#define USER_H
#include <iostream>
#include <vector>

#include "post.h"

using namespace std;

class User
{
public:
    int id = -1;
    string email = "";
    string fullname = "";
    string registerDate = "";
    string password = "";
    //bool is_admin = false;
    //vector<Post*> posts;

    User() { }
    ~User() { }
};

#endif // USER_H
