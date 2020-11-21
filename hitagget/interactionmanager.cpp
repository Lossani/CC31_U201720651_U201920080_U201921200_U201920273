#include "interactionmanager.h"

InteractionManager::InteractionManager() : ListController<PostInteraction*, int, int>(
   [](PostInteraction* interaction)
   {
       return interaction->id;
   },
   [](int value1, int value2)
   {
       return value1 == value2;
   },
   [](ofstream& file, PostInteraction* element)
   {
   //QMessageBox msg;
       //msg.setText("newPost->get_post_content()");
       //msg.exec();
       file << element->id << endl;
       file << element->authorId << endl;
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

}

InteractionManager::~InteractionManager()
{

}

void InteractionManager::addInteraction(int authorId, int postId, bool shared)
{
    PostInteraction* newInteraction;

    newInteraction->id = time(0);
    newInteraction->authorId = authorId;
    newInteraction->parentPostId = postId;
    newInteraction->shared = shared;

    time_t now = time(0);
    newInteraction->date = ctime(&now);
    newInteraction->date = newInteraction->date.erase(newInteraction->date.find_last_not_of("\t\n\v\f\r ") + 1);

    add_element(newInteraction);
}

list<PostInteraction*> InteractionManager::getPostInteractions(int postId)
{
    return avl_interactions_by_post_id->findAll(postId);
}

PostInteraction InteractionManager::getInteraction(int interactionId)
{
    return *get_element(interactionId);
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
