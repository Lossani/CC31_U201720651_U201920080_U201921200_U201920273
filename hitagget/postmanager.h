#ifndef POSTMANAGER_H
#define POSTMANAGER_H
#include <iostream>
#include "post.h"
#include "listcontroller.h"
//#include "commentmanager.h"
#include <list>

using namespace std;

class PostManager : private ListController<Post*, int, int>
{
private:
    //CommentManager all_comments;
public:
    PostManager();
    ~PostManager();

    //void add_post(Post post);
    virtual void addPost(int authorId, string title, string content);
    void addPost(Post* post);
    void updatePost(Post* post);
    void deletePost(int postId);
    list<Post*> getAuthorPosts(int userId);
    list<Post*> getAllPosts();
    Post* getPost(int postId);
   // void add_comment_to_post(int post_id, int comment_author_id, string comment);
    //void delete_comment(int comment_id);
    //list<PostComment> get_post_comments(int post_id);
};

#endif // POSTMANAGER_H
