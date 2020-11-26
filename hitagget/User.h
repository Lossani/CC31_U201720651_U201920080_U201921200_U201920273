#ifndef USER_H
#define USER_H
#include <iostream>
#include <vector>

#include "post.h"

using namespace std;


struct Follower
{
public:
    int userID = -1;
    int followedUserID = -1;
    bool isDeleted = false;

    Follower() { }
    ~Follower() { }
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

    bool isFollowing(int userId)
    {
        for(Follower* follower : followedUsers)
        {
            if (follower->followedUserID == userId)
                return true;
        }

        return false;
    }
};

#endif // USER_H
