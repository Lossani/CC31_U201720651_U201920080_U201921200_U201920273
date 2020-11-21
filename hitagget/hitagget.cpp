#include "hitagget.h"
#include <iostream>
#include <QMessageBox>

using namespace std;

Hitagget::Hitagget() : UserManager(), PostManager(), CommentManager()
{

}

Hitagget::~Hitagget()
{

}

bool Hitagget::log_in(string email, string password)
{
    User user = *getUserByEmail(email);

    if (user.email != "" && password == user.password)
    {
        logged_user = user;
        return true;
    }
    else
    {
        return false;
    }
}

bool Hitagget::sign_up(string email, string fullname, string password)
{
    if (!addUser(email, fullname, password))
    {
        return false;
    }
    else
    {
        logged_user = *getUserByEmail(email);
        return true;
    }
}
/*
void Hitagget::add_post(string content)
{
    PostManager::add_post(logged_user.id, content);
}*/
/*
void Hitagget::add_blog(char *title, char *content)
{
    BlogManager::add_blog(logged_user.id, title, content);
}*/

User Hitagget::get_logged_user()
{
    return logged_user;
}


/*
void Hitagget::update_logged_user(string fullname, string email)
{
    if (fullname == "" || email == "")
    {
        return;
    }
    else
    {
        logged_user.fullname = fullname;
        logged_user.email = email;
        update_user(logged_user);
    }
}
*/
