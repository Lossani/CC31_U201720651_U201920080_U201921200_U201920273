#ifndef FOLLOWERMANAGER_H
#define FOLLOWERMANAGER_H

#include "listcontroller.h"
#include "user.h"
#include "avl.h"


class FollowerManager : private ListController<Follower*, int, int>
{
private:
    AVL<Follower*, int, nullptr>* avl_followers_by_user_id;

public:
    FollowerManager();
    ~FollowerManager();

    list<Follower*> getUserFollowedUsers(int baseUserId);

    Follower* addFollower(int baseUserId, int followedUserId);

    void deleteFollower(Follower* follower);

    void saveFollowers();
};

#endif // FOLLOWERMANAGER_H
