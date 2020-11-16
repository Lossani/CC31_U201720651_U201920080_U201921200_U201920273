#ifndef INTERACTIONMANAGER_H
#define INTERACTIONMANAGER_H

#include "listcontroller.h"
#include "post.h"

class InteractionManager : private ListController<PostInteraction*, int, int>
{
public:
    InteractionManager();
    ~InteractionManager();

    void addInteraction(int authorId, int postId, bool shared);
    list<PostInteraction*> getPostInteractions(int postId);
    PostInteraction getInteraction(int interactionId);
};

#endif // INTERACTIONMANAGER_H
