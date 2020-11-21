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
   [this](ifstream& file)
   {
       avl_comments_by_post_id = new AVL<PostComment*, int, nullptr>([] (PostComment* element) { return element->parentPostId; });
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
           //currentComment->authorId = stoi(authorId);
           currentComment->parentPostId = stoi(publicationId);

           retrievedElements.push_back(currentComment);

           avl_comments_by_post_id->add(currentComment);
       }

       return retrievedElements;
   },
   "comment.tsv")
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
    return avl_comments_by_post_id->findAll(postId);
}

PostComment CommentManager::getComment(int commentId)
{
    return *get_element(commentId);
}
