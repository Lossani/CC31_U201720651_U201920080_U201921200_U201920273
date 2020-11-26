#ifndef HITAGGET_H
#define HITAGGET_H
#include <iostream>
#include "usermanager.h"
#include "postmanager.h"
#include "commentmanager.h"
#include "interactionmanager.h"
#include "trend.h"

#include <list>

using namespace std;

class Hitagget : public UserManager, public PostManager, public CommentManager
{
private:
    User* shown_user = nullptr;
    list<Post*> shown_user_posts;
public:
    Hitagget();
    ~Hitagget();

    User* logged_user;

    bool log_in(string email);
    void log_out();
    bool sign_up(string email, string fullname, string password);

    void save_instance();

    Post* addPost(int authorId, string title, string content) override;

    list<Post*> getShownUserPostsThatContainsString(string value, bool asc, int limit);
    list<Post*> getShownUserPostsNoContainsString(string value, bool asc, int limit);
    list<Post*> getShownUserPostsThatStartsWithString(string value, bool asc, int limit);
    list<Post*> getShownUserPostsThatEndsWithString(string value, bool asc, int limit);
    list<Post*> getShownUserPostsThatTitleEqualsToString(string value, bool asc, int limit);

    list<Post*> getShownUserPostsByPubDate(bool asc, int limit);
    list<Post*> getShownUserPostsByLikes(bool asc, int limit);
    list<Post*> getShownUserPostsByNumInteractions(bool asc, int limit);

    User* get_shown_user();
    list<Post*> get_shown_user_posts();
    void set_shown_user(int user_id);
    void set_shown_user(User* user);
    void clear_shown_user();

    list<Trend*> get_latest_trends(int limit);
};

#endif // HITAGGET_H
