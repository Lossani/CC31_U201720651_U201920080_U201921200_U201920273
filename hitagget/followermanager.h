#ifndef FOLLOWERMANAGER_H
#define FOLLOWERMANAGER_H

#include "ListController.h"
#include "User.h"
#include "avl.h"


class FollowerManager : private ListController<Follower*, int, int>
{
private:
    AVL<Follower*, int, nullptr>* avl_followers_by_user_id;
public:
    FollowerManager();
    ~FollowerManager();

    list<Follower*> getUserFollowedUsersIds(int baseUserId);
};

#endif // FOLLOWERMANAGER_H
