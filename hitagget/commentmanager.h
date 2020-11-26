#ifndef COMMENTMANAGER_H
#define COMMENTMANAGER_H

#include "listcontroller.h"
#include "avl.h"
#include "post.h"

class CommentManager : private ListController<PostComment*, int, int>
{
private:
    AVL<PostComment*, int, nullptr>* avl_comments_by_post_id;
public:
    CommentManager();
    ~CommentManager();

    void addComment(int postId, string content);
    void updateComment(PostComment comment);
    void deleteComment(int id);
    list<PostComment*> getPostComments(int postId);
    PostComment* getComment(int commentId);

    void saveComments();
};

#endif // COMMENTMANAGER_H
