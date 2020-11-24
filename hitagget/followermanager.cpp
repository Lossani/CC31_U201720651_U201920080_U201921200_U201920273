#include "followermanager.h"
#include <vector>
#include <list>

FollowerManager::FollowerManager() : ListController<Follower*, int, int>(
    [](Follower* follower)
    {
        return follower->userID;
    },
    [](int value1, int value2)
    {
        return value1 == value2;
    },
    [](ofstream& file, Follower* element)
    {
        file << element->userID << endl;
        file << element->followedUserID << endl;
    },
    [this](ifstream& file)
    {
        avl_followers_by_user_id = new AVL<Follower*, int, nullptr>([](Follower* element) { return element->userID; });
        list<Follower*> retrievedElements;

        Follower* currentFollower;

        string userID, followedUserID;

        getline(file, userID); // Ignoring first line, temporarily using userID for getline parameter

        while (getline(file, userID, '\t'))
        {
            currentFollower = new Follower();

            //getline(file, authorId, ',');
            getline(file, followedUserID);

            currentFollower->userID = stoi(userID);
            currentFollower->followedUserID = stoi(followedUserID);

            retrievedElements.push_back(currentFollower);

            avl_followers_by_user_id->add(currentFollower);
         }

         return retrievedElements;
    },
    "followers.tsv")
{

}

FollowerManager::~FollowerManager()
{

}

list<Follower*> FollowerManager::getUserFollowedUsersIds(int baseUserId)
{
    return avl_followers_by_user_id->findAll(baseUserId);
}
