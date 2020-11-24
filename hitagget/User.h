#ifndef USER_H
#define USER_H
#include <iostream>
#include <vector>

#include "post.h"

using namespace std;


struct Follower
{
public:
    int userID;
    int followedUserID;
};

class User
{
public:
    int id = -1;
    string email = "";
    string fullname = "";
    string registerDate = "";
    string password = "";

    list<Follower*> followedUsers;
    //bool is_admin = false;
    //vector<Post*> posts;

    User() { }
    ~User() { }
};

#endif // USER_H
