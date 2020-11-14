#ifndef POSTMANAGER_H
#define POSTMANAGER_H
#include <iostream>
#include "post.h"
#include "listcontroller.h"
//#include "commentmanager.h"
#include <list>

using namespace std;

class PostManager : private ListController<Post, int, int>
{
private:
    //CommentManager all_comments;
public:
    PostManager();
    ~PostManager();

    //void add_post(Post post);
    virtual void add_post(int author_id, string title, string content, string tag);
    void add_post(Post post);
    void update_post(Post updated_element);
    void delete_post(int post_id);
    list<Post> get_author_posts(int user_id);
    list<Post> get_all_posts();
    Post get_post(int post_id);
   // void add_comment_to_post(int post_id, int comment_author_id, string comment);
    //void delete_comment(int comment_id);
    //list<PostComment> get_post_comments(int post_id);
};

#endif // POSTMANAGER_H
