#ifndef COMMENTMANAGER_H
#define COMMENTMANAGER_H

#include "listcontroller.h"
#include "post.h"

class CommentManager : private ListController<PostComment*, int, int>
{
public:
    CommentManager();
    ~CommentManager();

    void addComment(int authorId, int postId, string content);
    void addComment(PostComment comment);
    void updateComment(PostComment comment);
    void deleteComment(int id);
    list<PostComment*> getPostComments(int postId);
    PostComment getComment(int commentId);
};

#endif // COMMENTMANAGER_H
