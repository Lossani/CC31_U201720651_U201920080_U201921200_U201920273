#ifndef POSTMANAGER_H
#define POSTMANAGER_H
#include <iostream>
#include "post.h"
#include "listcontroller.h"
#include "avl.h"
#include "trend.h"
//#include "commentmanager.h"
#include "interactionmanager.h"
#include <list>

using namespace std;

class PostManager : public InteractionManager, private ListController<Post*, int, int>
{
private:
    //CommentManager all_comments;
    AVL<Post*, int, nullptr>* avl_posts_by_id;
    AVL<Post*, int, nullptr>* avl_posts_by_authorId;
    AVL<Post*, string, nullptr>* avl_posts_by_title;
    AVL<Post*, string, nullptr>*  avl_posts_by_pubDate;
    AVL<Post*, int, nullptr>* avl_posts_by_numLikes;
    AVL<Post*, int, nullptr>* avl_posts_by_numInteractions;

    AVL<string*, string, nullptr>* avl_trends;

public:
    PostManager();
    ~PostManager();

    list<Trend*>* all_trends;

    //void add_post(Post post);
    virtual Post* addPost(int authorId, string title, string content);

    void updatePost(Post* post);
    void deletePost(int postId);
    list<Post*> getAuthorPosts(int userId);
    list<Post*> getAuthorPosts(int userId, bool asc, int limit);
    list<Post*> getAuthorPostsByPubDate(int userId, bool asc, int limit);
    list<Post*> getAuthorPostsByLikes(int userId, bool asc, int limit);
    list<Post*> getAuthorPostsByNumInteractions(int userId, bool asc, int limit);
    list<Post*> getAllPosts();

    list<Post*> getAllPostsByPubDate(bool asc);
    list<Post*> getAllPostsByLikes(bool asc);
    list<Post*> getAllPostsByNumInteractions(bool asc);

    list<Post*> getPostsByPubDate(bool asc, int limit);
    list<Post*> getPostsByLikes(bool asc, int limit);
    list<Post*> getPostsByTitle(bool asc, int limit);

    list<Post*> getPostsThatContainsString(string value, bool asc, int limit);
    list<Post*> getPostsNoContainsString(string value, bool asc, int limit);
    list<Post*> getPostsThatStartsWithString(string value, bool asc, int limit);
    list<Post*> getPostsThatEndsWithString(string value, bool asc, int limit);
    list<Post*> getPostsThatTitleEqualsToString(string value, bool asc, int limit);

    list<Post*> getAuthorPostsThatContainsString(int userId, string value, bool asc, int limit);
    list<Post*> getAuthorPostsNoContainsString(int userId, string value, bool asc, int limit);
    list<Post*> getAuthorPostsThatStartsWithString(int userId, string value, bool asc, int limit);
    list<Post*> getAuthorPostsThatEndsWithString(int userId, string value, bool asc, int limit);
    list<Post*> getAuthorPostsThatTitleEqualsToString(int userId, string value, bool asc, int limit);


    Post* getPostById(int postId);

    list<Post*> getPostsByNumInteractions(bool asc, int limit);

    void savePosts();

    void updatePostsAVLs(Post* post);
};

#endif // POSTMANAGER_H
