#include <iostream>
#include "postmanager.h"
#include <fstream>
#include <QMessageBox>
#include <QTime>

using namespace std;

PostManager::PostManager() : InteractionManager(), ListController<Post*, int, int>(
    [](Post* post)
    {
        return post->id;
    },
    [](int value1, int value2)
    {
        return value1 == value2;
    },
    [this]()
    {
         ofstream file;

         file.open("publications.tsv", ios::out);

         if (file.is_open())
         {
             file << "id	idU	title	desc	pubdate	numlikes" << endl;

             for (Post* post : ListController<Post*, int, int>::get_all_elements())
             {
                 file << post->id << '\t'
                      << post->authorId << '\t'
                      << post->title << '\t'
                      << post->content << '\t'
                      << post->pubDate << '\t'
                      << post->numLikes << '\t'
                      << endl;
             }

             file.close();
         }
    },
    [this](ifstream& file)
    {
        avl_posts_by_id = new AVL<Post*, int, nullptr>([](Post* element) { return element->id; });
        avl_posts_by_authorId = new AVL<Post*, int, nullptr>([](Post* element) { return element->authorId; });
        avl_posts_by_title = new AVL<Post*, string, nullptr>([](Post* element) { return element->title; });
        avl_posts_by_pubDate = new AVL<Post*, string, nullptr>([](Post* element) { return element->pubDate; });
        avl_posts_by_numLikes = new AVL<Post*, int, nullptr>([](Post* element) { return element->numLikes; });
        avl_posts_by_numInteractions = new AVL<Post*, int, nullptr>([](Post* element) { return element->numInteractions; });

        avl_trends = new AVL<string*, string, nullptr>([](string* element) { return *element; });

        all_trends = new list<Trend*>();
        QStringList unique_word_list;

        list<Post*> retrievedElements;

        Post* currentPost;

        string id, authorId, numLikes, title;

        getline(file, id); // Ignoring first line, temporarily using id for getline parameter

        while (getline(file, id, '\t'))
        {
            currentPost = new Post();

            getline(file, authorId, '\t');
            getline(file, title, '\t');
            getline(file, currentPost->content, '\t');
            getline(file, currentPost->pubDate, '\t');
            getline(file, numLikes);

            currentPost->id = stoi(id);
            currentPost->authorId = stoi(authorId);
            currentPost->title = title;
            currentPost->numLikes = stoi(numLikes);
            currentPost->numInteractions = getNumInteractionsOfPost(stoi(id));

            retrievedElements.push_back(currentPost);

            avl_posts_by_id->add(currentPost);
            avl_posts_by_authorId->add(currentPost);
            avl_posts_by_title->add(currentPost);
            avl_posts_by_pubDate->add(currentPost);
            avl_posts_by_numLikes->add(currentPost);
            avl_posts_by_numInteractions->add(currentPost);

            QString linea = QString::fromStdString(title);

            QStringList words = linea.split(" ");

            words.removeDuplicates();

            for(QString word : words)
            {
                if (word.size() > 1 && word[0] == '#')
                {
                    avl_trends->add(new string(word.toStdString()));
                    unique_word_list.push_back(word);
                }
            }
        }

        unique_word_list.removeDuplicates();

        for (QString word : unique_word_list)
        {
            int count = avl_trends->findAll(word.toStdString()).size();
            if (count >= 0)
            {
                Trend* newTrend = new Trend(count, word.toStdString());
                all_trends->push_back(newTrend);
            }
        }

        all_trends->sort([](const Trend* a, const Trend* b) { return a->count > b->count; });

        return retrievedElements;
    },
    "publications.tsv")
{

}

PostManager::~PostManager()
{

}

Post* PostManager::addPost(int authorId, string title, string content)
{
    Post* newPost = new Post();

    newPost->id = ListController<Post*, int, int>::get_num_elements() + 1;
    newPost->authorId = authorId;
    newPost->content = content;
    newPost->title = title;
    newPost->pubDate = QDateTime::currentDateTime().toString("yyyy-MM-dd").toStdString();

    ListController<Post*, int, int>::add_element(newPost);

    avl_posts_by_id->add(newPost);
    avl_posts_by_authorId->add(newPost);
    avl_posts_by_title->add(newPost);
    avl_posts_by_pubDate->add(newPost);
    avl_posts_by_numLikes->add(newPost);
    avl_posts_by_numInteractions->add(newPost);

    QString linea = QString::fromStdString(title);

    QStringList words = linea.split(" ");
    QStringList foundTags;

    for(QString word : words)
    {
        if (word.size() > 1 && word[0] == '#')
        {
           foundTags.push_back(word);
        }
    }

    foundTags.removeDuplicates();

    for (QString word : foundTags)
    {
        bool found = false;
        for (Trend* trend : *all_trends)
        {
            if (trend->tag == word.toStdString())
            {
                trend->count++;
                found = true;
            }
        }

        if (!found)
        {
            all_trends->push_back(new Trend(1, word.toStdString()));
        }
    }

    if (foundTags.size() > 0)
        all_trends->sort([](const Trend* a, const Trend* b) { return a->count > b->count; });

    return newPost;
}

void PostManager::updatePost(Post* post)
{
    ListController<Post*, int, int>::update_element(post);
}

void PostManager::deletePost(int id)
{
    ListController<Post*, int, int>::delete_element(id);
}

list<Post*> PostManager::getAuthorPosts(int userId)
{
    return avl_posts_by_authorId->findAll(userId);
}

list<Post *> PostManager::getAuthorPosts(int userId, bool asc, int limit)
{
    if (limit <= 0)
        return list<Post*>();

    return avl_posts_by_authorId->findAll(userId, asc, limit);
}

list<Post *> PostManager::getAuthorPostsByPubDate(int userId, bool asc, int limit)
{
    if (limit <= 0)
        return list<Post*>();

    list<Post*> returnList = avl_posts_by_authorId->findAll(userId, asc, limit);
    if (asc)
        returnList.sort([](const Post* a, const Post* b) { return a->pubDate < b->pubDate; });
    else
        returnList.sort([](const Post* a, const Post* b) { return a->pubDate > b->pubDate; });
    return returnList;
}

list<Post *> PostManager::getAuthorPostsByLikes(int userId, bool asc, int limit)
{
    if (limit <= 0)
        return list<Post*>();

    list<Post*> returnList = avl_posts_by_authorId->findAll(userId, asc, limit);
    if (asc)
        returnList.sort([](const Post* a, const Post* b) { return a->numLikes < b->numLikes; });
    else
        returnList.sort([](const Post* a, const Post* b) { return a->numLikes > b->numLikes; });
    return returnList;
}

list<Post *> PostManager::getAuthorPostsByNumInteractions(int userId, bool asc, int limit)
{
    if (limit <= 0)
        return list<Post*>();

    list<Post*> returnList = avl_posts_by_authorId->findAll(userId, asc, limit);
    if (asc)
        returnList.sort([](const Post* a, const Post* b) { return a->numInteractions < b->numInteractions; });
    else
        returnList.sort([](const Post* a, const Post* b) { return a->numInteractions > b->numInteractions; });
    return returnList;
}

/*
void PostManager::save_posts()
{
    ofstream posts_file;
    ofstream comments_file;

    posts_file.open("posts.dat", ios::out | ios::binary | ios::app);
    comments_file.open("comments.dat", ios::out | ios::binary | ios::app);

    if (posts_file.is_open() && comments_file.is_open())
    {
        for (Post post : all_posts)
        {
            posts_file.write((const char*) &post.id, sizeof(Post::id));
            posts_file.write((const char*) &post.author, sizeof(Post::author));
            posts_file.write((const char*) &post.content, sizeof(Post::content));
            posts_file.write((const char*) &post.comments_number, sizeof(Post::comments_number));

            for (Post comment : post.comments)
            {
                comments_file.write((const char*) &comment, sizeof(Post));
            }
        }

        posts_file.close();
        comments_file.close();
    }
}

void PostManager::retrieve_posts()
{
    ifstream posts_file;
    ifstream comments_file;

    posts_file.open("posts.dat", ios::in | ios::binary);
    comments_file.open("comments.dat", ios::in | ios::binary);

    if (posts_file.is_open() && comments_file.is_open())
    {
        posts_file.seekg(0, ios::end);
        int posts_number = posts_file.tellg() / sizeof(Post);
        int last_post_comments_number = 0;

        for (int i = 0; i < posts_number; ++i)
        {
            Post current_post;
            posts_file.seekg(i * sizeof(Post), ios::beg);
            posts_file.read((char*)&current_post.id, sizeof(Post::id));
            posts_file.read((char*)&current_post.author, sizeof(Post::author));
            posts_file.read((char*)&current_post.content, sizeof(Post::content));
            posts_file.read((char*)&current_post.comments_number, sizeof(Post::comments_number));

            for (int j = 0; j < current_post.comments_number; ++j)
            {
                Post current_comment;
                comments_file.seekg((last_post_comments_number + j) * sizeof(Post), ios::beg);
                comments_file.read((char*)&current_comment, sizeof(Post));
                current_post.comments.push_back(current_comment);
            }

            last_post_comments_number = current_post.comments.size();
            all_posts.push_back(current_post);
        }

        posts_file.close();
    }
}
*/
list<Post*> PostManager::getAllPosts()
{
    return ListController<Post*, int, int>::get_all_elements();
}

list<Post *> PostManager::getAllPostsByPubDate(bool asc)
{
    if (asc)
        return avl_posts_by_pubDate->inOrder();
    else
        return avl_posts_by_pubDate->postOrder();
}

list<Post *> PostManager::getAllPostsByLikes(bool asc)
{
    if (asc)
        return avl_posts_by_numLikes->inOrder();
    else
        return avl_posts_by_numLikes->postOrder();
}

list<Post *> PostManager::getAllPostsByNumInteractions(bool asc)
{
    if (asc)
        return avl_posts_by_numInteractions->inOrder();
    else
        return avl_posts_by_numInteractions->postOrder();
}

list<Post *> PostManager::getPostsByPubDate(bool asc, int limit)
{
    if (limit <= 0)
        return list<Post*>();

    if (asc)
        return avl_posts_by_pubDate->inOrder(limit);
    else
        return avl_posts_by_pubDate->postOrder(limit);
}

list<Post *> PostManager::getPostsByLikes(bool asc, int limit)
{
    if (limit <= 0)
        return list<Post*>();

    if (asc)
        return avl_posts_by_numLikes->inOrder(limit);
    else
        return avl_posts_by_numLikes->postOrder(limit);
}

list<Post *> PostManager::getPostsByTitle(bool asc, int limit)
{
    if (limit <= 0)
        return list<Post*>();

    if (asc)
        return avl_posts_by_title->inOrder(limit);
    else
        return avl_posts_by_title->postOrder(limit);
}

list<Post *> PostManager::getPostsThatContainsString(string value, bool asc, int limit)
{
    if (limit <= 0)
        return list<Post*>();

    return avl_posts_by_numLikes->findAllStringsThatContains(value, asc, limit);
}

list<Post *> PostManager::getPostsNoContainsString(string value, bool asc, int limit)
{
    if (limit <= 0)
        return list<Post*>();

    return avl_posts_by_numLikes->findAllStringsNoContains(value, asc, limit);
}

list<Post *> PostManager::getPostsThatStartsWithString(string value, bool asc, int limit)
{
    if (limit <= 0)
        return list<Post*>();

    list<Post*> returnList = avl_posts_by_title->findAllStringsThatStartsWith(value, limit);
    if (asc)
        returnList.sort([](const Post* a, const Post* b) { return a->numLikes < b->numLikes; });
    else
        returnList.sort([](const Post* a, const Post* b) { return a->numLikes > b->numLikes; });
    return returnList;
}

list<Post *> PostManager::getPostsThatEndsWithString(string value, bool asc, int limit)
{
    if (limit <= 0)
        return list<Post*>();
    list<Post*> returnList = avl_posts_by_title->findAllStringsThatEndsWith(value, limit);
    if (asc)
        returnList.sort([](const Post* a, const Post* b) { return a->numLikes < b->numLikes; });
    else
        returnList.sort([](const Post* a, const Post* b) { return a->numLikes > b->numLikes; });
    return returnList;
}

list<Post *> PostManager::getPostsThatTitleEqualsToString(string value, bool asc, int limit)
{
    if (limit <= 0)
        return list<Post*>();

    list<Post*> returnList = avl_posts_by_title->findAll(value, limit);
    if (asc)
        returnList.sort([](const Post* a, const Post* b) { return a->title < b->title; });
    else
        returnList.sort([](const Post* a, const Post* b) { return a->title > b->title; });
    return returnList;
}

list<Post *> PostManager::getAuthorPostsThatContainsString(int userId, string value, bool asc, int limit)
{
    if (limit <= 0)
        return list<Post*>();

    list<Post*> returnList;

    for (Post* post : getAuthorPosts(userId))
    {
        if (returnList.size() >= (uint)limit)
            break;

        if (post->title.find(value) != string::npos)
        {
            returnList.push_back(post);
        }
    }

    if (asc)
        returnList.sort([](const Post* a, const Post* b) { return a->title < b->title; });
    else
        returnList.sort([](const Post* a, const Post* b) { return a->title > b->title; });

    return returnList;
}

list<Post *> PostManager::getAuthorPostsNoContainsString(int userId, string value, bool asc, int limit)
{
    if (limit <= 0)
        return list<Post*>();

    list<Post*> returnList;

    for (Post* post : getAuthorPosts(userId))
    {
        if (returnList.size() >= (uint)limit)
            break;

        if (post->title.find(value) == string::npos)
        {
            returnList.push_back(post);
        }
    }

    if (asc)
        returnList.sort([](const Post* a, const Post* b) { return a->title < b->title; });
    else
        returnList.sort([](const Post* a, const Post* b) { return a->title > b->title; });

    return returnList;
}

list<Post *> PostManager::getAuthorPostsThatStartsWithString(int userId, string value, bool asc, int limit)
{
    if (limit <= 0)
        return list<Post*>();

    list<Post*> returnList;

    for (Post* post : getAuthorPosts(userId))
    {
        if (returnList.size() >= (uint)limit)
            break;

        if (value.size() <= post->title.size())
        {
            if (value == post->title.substr(0, value.size()))
            {
                returnList.push_back(post);
            }
        }
    }

    if (asc)
        returnList.sort([](const Post* a, const Post* b) { return a->title < b->title; });
    else
        returnList.sort([](const Post* a, const Post* b) { return a->title > b->title; });

    return returnList;
}

list<Post *> PostManager::getAuthorPostsThatEndsWithString(int userId, string value, bool asc, int limit)
{
    if (limit <= 0)
        return list<Post*>();

    list<Post*> returnList;

    for (Post* post : getAuthorPosts(userId))
    {
        if (returnList.size() >= (uint)limit)
            break;

        if (value.size() <= post->title.size())
        {
            if (value == post->title.substr(post->title.size() - value.size(), value.size()))
            {
                returnList.push_back(post);
            }
        }
    }

    if (asc)
        returnList.sort([](const Post* a, const Post* b) { return a->title < b->title; });
    else
        returnList.sort([](const Post* a, const Post* b) { return a->title > b->title; });

    return returnList;
}

list<Post *> PostManager::getAuthorPostsThatTitleEqualsToString(int userId, string value, bool asc, int limit)
{
    if (limit <= 0)
        return list<Post*>();

    list<Post*> returnList;

    for (Post* post : getAuthorPosts(userId))
    {
        if (returnList.size() >= (uint)limit)
            break;

        if (post->title == value)
        {
            returnList.push_back(post);
        }
    }

    if (asc)
        returnList.sort([](const Post* a, const Post* b) { return a->title < b->title; });
    else
        returnList.sort([](const Post* a, const Post* b) { return a->title > b->title; });

    return returnList;
}

Post* PostManager::getPostById(int postId)
{
    return avl_posts_by_id->find(postId);
}

list<Post *> PostManager::getPostsByNumInteractions(bool asc, int limit)
{
    if (limit <= 0)
        return list<Post*>();

    if (asc)
        return avl_posts_by_numInteractions->inOrder(limit);
    else
        return avl_posts_by_numInteractions->postOrder(limit);
}

void PostManager::savePosts()
{
    ListController<Post*, int, int>::save_elements();
}
/*
void PostManager::add_comment_to_post(int post_id, int author_id, char* comment)
{
    PostComment new_comment;

    new_comment.id = time(0);
    new_comment.parent_post_id = post_id;
    new_comment.author_id = author_id;
    strcpy(new_comment.content, comment);

    all_comments.add_comment(new_comment);
}

void PostManager::delete_comment(int comment_id)
{
    all_comments.delete_comment(comment_id);
}

list<PostComment> PostManager::get_post_comments(int post_id)
{
    function<int(PostComment)> field_comparator = [](PostComment comment)
    {
        return comment.parent_post_id;
    };

    return all_comments.find_all_elements(field_comparator, post_id);
}*/

/*
void PostManager::add_comment_to_post(int post_id, int comment_author_id, char* comment)
{
    if (all_posts.size() == 0)
        return;

    Post post_to_update = get_post_by_id(post_id);
    Post new_comment;

    new_comment.author_id = comment_author_id;
    strcpy(new_comment.content, comment);
    new_comment.id = time(0);

    post_to_update.comments.push_back(new_comment);
    post_to_update.comments_number = post_to_update.comments.size();
    save_posts();
}

void PostManager::delete_comment(int post_id, int comment_id)
{
    int position = get_post_position(post_id);

    if (position == -1)
        return;

    int counter = 0;
    bool found = false;

    for (Post comment: all_posts[position].comments)
    {
        if (comment.id == comment_id)
        {
            found = true;
            break;
        }
        counter++;
    }

    if (found)
    {
        all_posts[position].comments.erase(all_posts[position].comments.begin() + counter);
        save_posts();
    }
}

int PostManager::get_post_position(int post_id)
{
    int counter = 0;
    for (Post post : all_posts)
    {
        if (post.id == post_id)
        {
           return counter;
        }
    }

    return -1;
}
*/
