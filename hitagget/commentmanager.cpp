#include "commentmanager.h"
#include <QTime>

CommentManager::CommentManager() : ListController<PostComment*, int, int>(
   [](PostComment* comment)
   {
       return comment->id;
   },
   [](int value1, int value2)
   {
       return value1 == value2;
   },
   [this]()
   {
       ofstream file;

       file.open("comment.tsv", ios::out);

       if (file.is_open())
       {
           file << "id    idPub   date    text" << endl;

           for (PostComment* comment : ListController<PostComment*, int, int>::get_all_elements())
           {
               if (!comment->isDeleted)
                   file << comment->id << '\t'
                        << comment->parentPostId << '\t'
                        << comment->pubDate << '\t'
                        << comment->content
                        << endl;
           }

           file.close();
       }
   },
   [this](ifstream& file)
   {
       avl_comments_by_post_id = new AVL<PostComment*, int, nullptr>([] (PostComment* element) { return element->parentPostId; });

       currentIndex = 1;

       list<PostComment*> retrievedElements;

       PostComment* currentComment;

       string id, authorId, publicationId;

       getline(file, id); // Ignoring first line, temporarily using id for getline parameter

       while (getline(file, id, '\t'))
       {
           currentComment = new PostComment();

           //getline(file, id, ','); // Retrieving the real ID of the comment
           //getline(file, authorId, ','); // We have no author ID in the files yet
           getline(file, publicationId, '\t');
           getline(file, currentComment->pubDate, '\t');
           getline(file, currentComment->content);

           currentComment->id = stoi(id);
           currentComment->parentPostId = stoi(publicationId);

           if (currentComment->id > currentIndex)
               currentIndex = currentComment->id;

           retrievedElements.push_back(currentComment);

           avl_comments_by_post_id->add(currentComment);
       }

       return retrievedElements;
   },
   "comment.tsv")
{
    /*if (avl_comments_by_post_id == nullptr)
    {
         avl_comments_by_post_id = new AVL<PostComment*, int, nullptr>([] (PostComment* element) { return element->parentPostId; });
    }*/
}

CommentManager::~CommentManager()
{

}

void CommentManager::addComment(int postId, string content)
{
    PostComment* newComment = new PostComment();

    newComment->id = ++currentIndex;
    newComment->parentPostId = postId;
    newComment->content = content;
    newComment->pubDate = QDateTime::currentDateTime().toString("yyyy-MM-dd").toStdString();

    add_element(newComment);

    avl_comments_by_post_id->add(newComment);
}

void CommentManager::deletePostComments(int postId)
{
    for (PostComment* comment : avl_comments_by_post_id->findAll(postId))
    {
        comment->isDeleted = true;
        avl_comments_by_post_id->remove(comment);
    }

    //avl_comments_by_post_id->removeAll(postId);
}

list<PostComment*> CommentManager::getPostComments(int postId)
{
    return avl_comments_by_post_id->findAll(postId);
}

PostComment* CommentManager::getComment(int commentId)
{
    return get_element(commentId);
}

void CommentManager::saveComments()
{
    save_elements();
}
