#ifndef HITAGGET_H
#define HITAGGET_H
#include <iostream>
#include "usermanager.h"
#include "postmanager.h"
#include "commentmanager.h"
#include "interactionmanager.h"
//#include "blogmanager.h"
#include <list>

using namespace std;

class Hitagget : public UserManager, public PostManager, public CommentManager
{
public:
    Hitagget();
    ~Hitagget();

    User* logged_user;
    User* shown_user;

    bool log_in(string email);
    //void log_out();
    bool sign_up(string email, string fullname, string password);

    void save_instance();
    //void add_post(string title, string content);
    //void add_blog(string title, string content);
    //User* get_logged_user();
    //void update_logged_user(string nickname, string email);
};

#endif // HITAGGET_H
