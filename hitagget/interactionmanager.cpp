#include "interactionmanager.h"
#include <QTime>

InteractionManager::InteractionManager() : ListController<PostInteraction*, int, int>(
   [](PostInteraction* interaction)
   {
       return interaction->id;
   },
   [](int value1, int value2)
   {
       return value1 == value2;
   },
   [this]()
   {
        ofstream file;

        file.open("interaction.tsv", ios::out);

        if (file.is_open())
        {
            file << "idUser	idPub	date	share" << endl;

            for (PostInteraction* interaction : get_all_elements())
            {
                if (!interaction->isDeleted)
                    file << interaction->authorId << '\t'
                         << interaction->parentPostId << '\t'
                         << interaction->date << '\t'
                         << (interaction->shared ? "TRUE" : "FALSE")
                         << endl;
            }

            file.close();
        }
   },
   [this](ifstream& file)
   {
       avl_interactions_by_post_id = new AVL<PostInteraction*, int, nullptr>([](PostInteraction* element) { return element->parentPostId; });
       list<PostInteraction*> retrievedElements;

       PostInteraction* currentInteraction;

       string id, authorId, publicationId, shared;

       int i = 0; // We have no id of the interactions in the files, so we use a basic counter just to fill that field.

       getline(file, id); // Ignoring first line, temporarily using id for getline parameter

       while (getline(file, authorId, '\t'))
       {
           currentInteraction = new PostInteraction();

           //getline(file, authorId, ',');
           getline(file, publicationId, '\t');
           getline(file, currentInteraction->date, '\t');
           getline(file, shared);

           currentInteraction->id = i++;
           currentInteraction->authorId = stoi(authorId);
           currentInteraction->parentPostId = stoi(publicationId);
           currentInteraction->shared = shared == "TRUE" ? true : false;

           retrievedElements.push_back(currentInteraction);

           avl_interactions_by_post_id->add(currentInteraction);
       }

       return retrievedElements;
   },
   "interaction.tsv")
{
    /*if (avl_interactions_by_post_id == nullptr)
    {
        avl_interactions_by_post_id = new AVL<PostInteraction*, int, nullptr>([](PostInteraction* element) { return element->parentPostId; });
    }*/
}

InteractionManager::~InteractionManager()
{

}

void InteractionManager::addInteraction(int authorId, Post* post, bool shared)
{
    PostInteraction* newInteraction = new PostInteraction();

    newInteraction->authorId = authorId;
    newInteraction->parentPostId = post->id;
    newInteraction->shared = shared;

    newInteraction->date = QDateTime::currentDateTime().toString("yyyy-MM-dd").toStdString();

    add_element(newInteraction);

    avl_interactions_by_post_id->add(newInteraction);

    post->numInteractions += shared ? 2 : 1;
}

void InteractionManager::editLastInteraction(bool shared)
{
    get_last_element()->shared = shared;
}

void InteractionManager::deletePostInteractions(int postId)
{
    for (PostInteraction* interaction : avl_interactions_by_post_id->findAll(postId))
    {
        interaction->isDeleted = true;
        avl_interactions_by_post_id->remove(interaction);
    }

    //avl_interactions_by_post_id->removeAll(postId);
}

list<PostInteraction*> InteractionManager::getPostInteractions(int postId)
{
    return avl_interactions_by_post_id->findAll(postId);
}

PostInteraction* InteractionManager::getInteraction(int interactionId)
{
    return get_element(interactionId);
}

int InteractionManager::getNumInteractionsOfPost(int postId)
{
    int numInteractions = 0;

    for (PostInteraction* interaction : avl_interactions_by_post_id->findAll(postId))
    {
        if (interaction->shared)
            numInteractions += 2;
        else
            numInteractions++;
    }

    return numInteractions;
}

void InteractionManager::saveInteractions()
{
    save_elements();
}
