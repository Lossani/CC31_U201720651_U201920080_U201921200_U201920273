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

bool Hitagget::log_in(string email)
{
    User* user = getUserByEmail(email);
    if (user == nullptr)
        return false;

    logged_user = user;

    return true;

}

void Hitagget::log_out()
{
    clear_shown_user();
    logged_user = nullptr;
}

bool Hitagget::sign_up(string email, string fullname, string password)
{
    if (!addUser(email, fullname, password))
    {
        return false;
    }
    else
    {
        logged_user = getUserByEmail(email);
        return true;
    }
}

void Hitagget::save_instance()
{
    saveUsers();
    savePosts();
    saveComments();
    saveInteractions();
    saveFollowers();
}

Post* Hitagget::addPost(int authorId, string title, string content)
{
    Post* newPost = PostManager::addPost(authorId, title, content);

    if (shown_user != nullptr)
    {
        if (shown_user->id == authorId)
        {
            shown_user_posts.push_back(newPost);
        }
    }

    return newPost;
}

list<Post *> Hitagget::getShownUserPostsThatContainsString(string value, bool asc, int limit)
{
    if (limit <= 0 || shown_user == nullptr || shown_user_posts.size() == 0)
        return list<Post*>();

    list<Post*> returnList;

    for (Post* post : shown_user_posts)
    {
        if (returnList.size() >= (uint)limit)
            break;

        if (post->title.find(value) != string::npos)
        {
            returnList.push_back(post);
        }
    }

    if (asc)
        returnList.sort([](const Post* a, const Post* b) { return a->title < b->title; });
    else
        returnList.sort([](const Post* a, const Post* b) { return a->title > b->title; });

    return returnList;
}

list<Post *> Hitagget::getShownUserPostsNoContainsString(string value, bool asc, int limit)
{
    if (limit <= 0 || shown_user == nullptr || shown_user_posts.size() == 0)
        return list<Post*>();

    list<Post*> returnList;

    for (Post* post : shown_user_posts)
    {
        if (returnList.size() >= (uint)limit)
            break;

        if (post->title.find(value) == string::npos)
        {
            returnList.push_back(post);
        }
    }

    if (asc)
        returnList.sort([](const Post* a, const Post* b) { return a->title < b->title; });
    else
        returnList.sort([](const Post* a, const Post* b) { return a->title > b->title; });

    return returnList;
}

list<Post *> Hitagget::getShownUserPostsThatStartsWithString(string value, bool asc, int limit)
{
    if (limit <= 0 || shown_user == nullptr || shown_user_posts.size() == 0)
        return list<Post*>();

    list<Post*> returnList;

    for (Post* post : shown_user_posts)
    {
        if (returnList.size() >= (uint)limit)
            break;

        if (value.size() <= post->title.size())
        {
            if (value == post->title.substr(0, value.size()))
            {
                returnList.push_back(post);
            }
        }
    }

    if (asc)
        returnList.sort([](const Post* a, const Post* b) { return a->title < b->title; });
    else
        returnList.sort([](const Post* a, const Post* b) { return a->title > b->title; });

    return returnList;
}

list<Post *> Hitagget::getShownUserPostsThatEndsWithString(string value, bool asc, int limit)
{
    if (limit <= 0 || shown_user == nullptr || shown_user_posts.size() == 0)
        return list<Post*>();

    list<Post*> returnList;

    for (Post* post : shown_user_posts)
    {
        if (returnList.size() >= (uint)limit)
            break;

        if (value.size() <= post->title.size())
        {
            if (value == post->title.substr(post->title.size() - value.size(), value.size()))
            {
                returnList.push_back(post);
            }
        }
    }

    if (asc)
        returnList.sort([](const Post* a, const Post* b) { return a->title < b->title; });
    else
        returnList.sort([](const Post* a, const Post* b) { return a->title > b->title; });

    return returnList;
}

list<Post *> Hitagget::getShownUserPostsThatTitleEqualsToString(string value, bool asc, int limit)
{
    if (limit <= 0 || shown_user == nullptr || shown_user_posts.size() == 0)
        return list<Post*>();

    list<Post*> returnList;

    for (Post* post : shown_user_posts)
    {
        if (returnList.size() >= (uint)limit)
            break;

        if (post->title == value)
        {
            returnList.push_back(post);
        }
    }

    if (asc)
        returnList.sort([](const Post* a, const Post* b) { return a->title < b->title; });
    else
        returnList.sort([](const Post* a, const Post* b) { return a->title > b->title; });

    return returnList;
}

list<Post *> Hitagget::getShownUserPostsByPubDate(bool asc, int limit)
{
    if (limit <= 0)
        return list<Post*>();

    list<Post*> userPosts = shown_user_posts;

    if (asc)
        userPosts.sort([](const Post* a, const Post* b) { return a->pubDate < b->pubDate; });
    else
        userPosts.sort([](const Post* a, const Post* b) { return a->pubDate > b->pubDate; });

    list<Post*> returnList;

    for (Post* post : userPosts)
    {
        if (returnList.size() >= (uint)limit)
            break;

        returnList.push_back(post);
    }

    return returnList;
}

list<Post *> Hitagget::getShownUserPostsByLikes(bool asc, int limit)
{
    if (limit <= 0)
        return list<Post*>();

    list<Post*> userPosts = shown_user_posts;

    if (asc)
        userPosts.sort([](const Post* a, const Post* b) { return a->numLikes < b->numLikes; });
    else
        userPosts.sort([](const Post* a, const Post* b) { return a->numLikes > b->numLikes; });

    list<Post*> returnList;

    for (Post* post : userPosts)
    {
        if (returnList.size() >= (uint)limit)
            break;

        returnList.push_back(post);
    }

    return returnList;
}

list<Post *> Hitagget::getShownUserPostsByNumInteractions(bool asc, int limit)
{
    if (limit <= 0)
        return list<Post*>();

    list<Post*> userPosts = shown_user_posts;

    if (asc)
        userPosts.sort([](const Post* a, const Post* b) { return a->numInteractions < b->numInteractions; });
    else
        userPosts.sort([](const Post* a, const Post* b) { return a->numInteractions > b->numInteractions; });

    list<Post*> returnList;

    for (Post* post : userPosts)
    {
        if (returnList.size() >= (uint)limit)
            break;

        returnList.push_back(post);
    }

    return returnList;
}

User *Hitagget::get_shown_user()
{
    return shown_user;
}

list<Post *> Hitagget::get_shown_user_posts()
{
    return shown_user_posts;
}

void Hitagget::set_shown_user(int user_id)
{
    shown_user = getUserById(user_id);
    shown_user_posts = getAuthorPosts(user_id);
}

void Hitagget::set_shown_user(User *user)
{
    shown_user = user;
    shown_user_posts = getAuthorPosts(user->id);
}

void Hitagget::clear_shown_user()
{
    shown_user = nullptr;
    shown_user_posts.clear();
}

list<Trend *> Hitagget::get_latest_trends(int limit)
{
    int count = 0;
    list<Trend*> returnList;

    if (all_trends != nullptr && all_trends->size() > 0)
    {
        for (Trend* trend : *all_trends)
        {
            if (count >= limit)
                break;

            returnList.push_back(trend);
            count++;
        }
    }

    return returnList;
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
