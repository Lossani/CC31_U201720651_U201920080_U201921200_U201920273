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
           currentInteraction->shared = shared == "TRUE" ? true : false;

           retrievedElements.push_back(currentInteraction);
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
