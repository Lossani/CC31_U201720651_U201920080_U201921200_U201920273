#include "hitagget.h"
#include <iostream>
#include <QMessageBox>

using namespace std;

Hitagget::Hitagget() : PostManager(), UserManager()
{

}

Hitagget::~Hitagget()
{

}

bool Hitagget::log_in(string email, string password)
{
    User user = get_user_by_email(email);

    if (user.email != "none" && password == user.password)
    {
        logged_user = user;
        all_logged_user_posts = get_author_posts(user.id);
        return true;
    }
    else
    {
        return false;
    }
}

bool Hitagget::sign_up(string email, string nickname, string password)
{
    if (!add_user(email, nickname, password))
    {
        return false;
    }
    else
    {
        logged_user = get_user_by_email(email);
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
