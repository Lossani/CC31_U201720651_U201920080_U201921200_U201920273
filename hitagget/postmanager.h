#ifndef POSTMANAGER_H
#define POSTMANAGER_H
#include <iostream>
#include "post.h"
#include "listcontroller.h"
#include "avl.h"
//#include "commentmanager.h"
#include "interactionmanager.h"
#include <list>

using namespace std;

class PostManager : private InteractionManager, private ListController<Post*, int, int>
{
private:
    //CommentManager all_comments;
    AVL<Post*, int, nullptr>* avl_posts_by_id;
    AVL<Post*, int, nullptr>* avl_posts_by_authorId;
    AVL<Post*, string, nullptr>* avl_posts_by_title;
    AVL<Post*, string, nullptr>*  avl_posts_by_pubDate;
    AVL<Post*, int, nullptr>* avl_posts_by_numLikes;
    AVL<Post*, int, nullptr>* avl_posts_by_numInteractions;

public:
    PostManager();
    ~PostManager();

    //void add_post(Post post);
    virtual void addPost(int authorId, string title, string content);
    void addPost(Post* post);
    void updatePost(Post* post);
    void deletePost(int postId);
    list<Post*> getAuthorPosts(int userId);
    list<Post*> getAuthorPosts(int userId, bool asc, int limit);
    list<Post*> getAllPosts();

    list<Post*> getAllPostsByPubDate(bool asc);
    list<Post*> getAllPostsByLikes(bool asc);
    list<Post *> getAllPostsByNumInteractions(bool asc);

    list<Post*> getPostsByPubDate(bool asc, int limit);
    list<Post*> getPostsByLikes(bool asc, int limit);
    list<Post*> getPostsByTitle(bool asc, int limit);

    list<Post*> getPostsThatContainsString(string value, bool asc, int limit);
    list<Post *> getPostsNoContainsString(string value, bool asc, int limit);
    list<Post*> getPostsThatStartsWithString(string value, bool asc, int limit);
    list<Post*> getPostsThatEndsWithString(string value, bool asc, int limit);
    list<Post*> getPostsThatTitleEqualsToString(string value, bool asc, int limit);

    Post* getPostById(int postId);

    list<Post*> getPostsByNumInteractions(bool asc, int limit);
   // void add_comment_to_post(int post_id, int comment_author_id, string comment);
    //void delete_comment(int comment_id);
    //list<PostComment> get_post_comments(int post_id);
};

#endif // POSTMANAGER_H
