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
   [](ifstream& file)
   {
       list<PostInteraction*> retrievedElements;

       PostInteraction* currentInteraction;

       string id, authorId, publicationId, shared;

       while (getline(file, id, ','))
       {
           currentInteraction = new PostInteraction();

           getline(file, authorId, ',');
           getline(file, publicationId, ',');
           getline(file, currentInteraction->date, ',');
           getline(file, shared);

           currentInteraction->id = stoi(id);
           currentInteraction->authorId = stoi(authorId);
           currentInteraction->shared = stoi(shared);

           retrievedElements.push_back(currentInteraction);
       }

       return retrievedElements;
   },
   "interactions.dat")
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
    function<int(PostInteraction*)> field_comparator = [](PostInteraction* interaction)
    {
        return interaction->parentPostId;
    };

    return find_elements(field_comparator, postId);
}

PostInteraction InteractionManager::getInteraction(int interactionId)
{
    get_element(interactionId);
}
