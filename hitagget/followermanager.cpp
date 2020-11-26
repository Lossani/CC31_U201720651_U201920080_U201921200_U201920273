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
    [this]()
    {
        ofstream file;

        file.open("followers.tsv", ios::out);

        if (file.is_open())
        {
            file << "idU	idF" << endl;

            for (Follower* follower : ListController<Follower*, int, int>::get_all_elements())
            {
                if (!follower->isDeleted)
                    file << follower->userID << '\t'
                         << follower->followedUserID
                         << endl;
            }

            file.close();
        }
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
    /*if (avl_followers_by_user_id == nullptr)
    {
        avl_followers_by_user_id = new AVL<Follower*, int, nullptr>([](Follower* element) { return element->userID; });
    }*/
}

FollowerManager::~FollowerManager()
{

}

list<Follower*> FollowerManager::getUserFollowedUsers(int baseUserId)
{
    return avl_followers_by_user_id->findAll(baseUserId);
}

Follower* FollowerManager::addFollower(int baseUserId, int followedUserId)
{
    Follower* newFollower = new Follower();

    newFollower->userID = baseUserId;
    newFollower->followedUserID = followedUserId;

    add_element(newFollower);

    avl_followers_by_user_id->add(newFollower);

    return newFollower;
}

void FollowerManager::deleteFollower(Follower *follower)
{
    avl_followers_by_user_id->remove(follower);
}

void FollowerManager::saveFollowers()
{
    save_elements();
}
