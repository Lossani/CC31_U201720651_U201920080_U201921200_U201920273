#include "commentmanager.h"

CommentManager::CommentManager() : ListController<PostComment*, int, int>(
   [](PostComment* comment)
   {
       return comment->id;
   },
   [](int value1, int value2)
   {
       return value1 == value2;
   },
   [](ofstream& file, PostComment* element)
   {
   //QMessageBox msg;
       //msg.setText("newPost->get_post_content()");
       //msg.exec();
       file << element->id << endl;
       file << element->authorId << endl;
       file << element->content << endl;
   },
   [](ifstream& file)
   {
       list<PostComment*> retrievedElements;

       PostComment* currentComment;

       string id, authorId, publicationId;

       while (getline(file, id, ','))
       {
           currentComment = new PostComment();

           getline(file, authorId, ',');
           getline(file, publicationId, ',');
           getline(file, currentComment->pubDate, ',');
           getline(file, currentComment->content);

           currentComment->id = stoi(id);
           currentComment->authorId = stoi(authorId);
           currentComment->parentPostId = stoi(publicationId);

           retrievedElements.push_back(currentComment);
       }

       return retrievedElements;
   },
   "comments.dat")
{

}

CommentManager::~CommentManager()
{

}

void CommentManager::addComment(int authorId, int postId, string content)
{
    PostComment* newComment;

    newComment->id = time(0);
    newComment->authorId = authorId;
    newComment->parentPostId = postId;
    newComment->content = content;
    time_t now = time(0);
    newComment->pubDate = ctime(&now);
    newComment->pubDate = newComment->pubDate.erase(newComment->pubDate.find_last_not_of("\t\n\v\f\r ") + 1);

    add_element(newComment);
}

void CommentManager::addComment(PostComment comment)
{
    comment.id = time(0);
    add_element(&comment);
}

void CommentManager::updateComment(PostComment comment)
{
    update_element(&comment);
}

void CommentManager::deleteComment(int id)
{
    delete_element(id);
}

list<PostComment*> CommentManager::getPostComments(int postId)
{
    function<int(PostComment*)> field_comparator = [](PostComment* comment)
    {
        return comment->parentPostId;
    };

    return find_elements(field_comparator, postId);
}

PostComment CommentManager::getComment(int commentId)
{
    return *get_element(commentId);
}
