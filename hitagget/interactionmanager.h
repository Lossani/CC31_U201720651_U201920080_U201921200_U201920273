#ifndef INTERACTIONMANAGER_H
#define INTERACTIONMANAGER_H

#include "listcontroller.h"
#include "avl.h"
#include "post.h"

class InteractionManager : private ListController<PostInteraction*, int, int>
{
private:
    AVL<PostInteraction*, int, nullptr>* avl_interactions_by_post_id;
public:
    InteractionManager();
    ~InteractionManager();

    void addInteraction(int authorId, Post* post, bool shared);
    void editLastInteraction(bool shared);
    list<PostInteraction*> getPostInteractions(int postId);
    PostInteraction* getInteraction(int interactionId);

    int getNumInteractionsOfPost(int postId);

    void saveInteractions();
};

#endif // INTERACTIONMANAGER_H
