#include <iostream>
#include "postmanager.h"
#include <fstream>
#include <QMessageBox>
#include <QTime>
#include <optional>

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
                 if (!post->isDeleted)
                     file << post->id << '\t'
                          << post->authorId << '\t'
                          << post->title << '\t'
                          << post->content << '\t'
                          << post->pubDate << '\t'
                          << post->numLikes
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

        currentIndex = 1;

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

            if (currentPost->id > currentIndex)
                currentIndex = currentPost->id;

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
         /*if (avl_posts_by_id == nullptr)
            avl_posts_by_id = new AVL<Post*, int, nullptr>([](Post* element) { return element->id; });
         if (avl_posts_by_authorId == nullptr)
            avl_posts_by_authorId = new AVL<Post*, int, nullptr>([](Post* element) { return element->authorId; });
         if (avl_posts_by_title == nullptr)
            avl_posts_by_title = new AVL<Post*, string, nullptr>([](Post* element) { return element->title; });
         if (avl_posts_by_pubDate == nullptr)
            avl_posts_by_pubDate = new AVL<Post*, string, nullptr>([](Post* element) { return element->pubDate; });
         if (avl_posts_by_numLikes == nullptr)
            avl_posts_by_numLikes = new AVL<Post*, int, nullptr>([](Post* element) { return element->numLikes; });
         if (avl_posts_by_numInteractions == nullptr)
            avl_posts_by_numInteractions = new AVL<Post*, int, nullptr>([](Post* element) { return element->numInteractions; });
         if (avl_trends == nullptr)
            avl_trends = new AVL<string*, string, nullptr>([](string* element) { return *element; });
         if (all_trends == nullptr)
            all_trends = new list<Trend*>();*/
}

PostManager::~PostManager()
{

}

Post* PostManager::addPost(int authorId, string title, string content)
{
    Post* newPost = new Post();

    newPost->id = ++currentIndex;
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

    updateTrendsFromPostTitle(newPost);

    return newPost;
}

void PostManager::updatePostsTitleAVL(Post *post)
{
    avl_posts_by_title->remove(post);
    avl_posts_by_title->add(post);
}

bool PostManager::updateTrendsFromPostTitle(Post *post)
{
    QString linea = QString::fromStdString(post->title);

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
    {
        all_trends->sort([](const Trend* a, const Trend* b) { return a->count > b->count; });
        return true;
    }
    else
        return false;
}

void PostManager::updateTrendsFromEditedPostTitle(Post *oldPost, Post *post)
{
    QString linea = QString::fromStdString(oldPost->title);
    QStringList words = linea.split(" ");

    QStringList foundTags;
    QStringList deletedTags;
    QStringList foundTagsInOldTitle;

    for(QString word : words)
    {
        if (word.size() > 1 && word[0] == '#')
        {
           foundTagsInOldTitle.push_back(word);
        }
    }

    foundTagsInOldTitle.removeDuplicates();

    linea = QString::fromStdString(post->title);

    words = linea.split(" ");

    for(QString word : words)
    {
        if (word.size() > 1 && word[0] == '#')
        {
           foundTags.push_back(word);
        }
    }

    foundTags.removeDuplicates();


    for (QString oldTag : foundTagsInOldTitle)
    {
        bool found = false;

        for (QString tag : foundTags)
        {
            if (tag == oldTag)
            {
                found = true;
                break;
            }
        }

        if (!found)
            deletedTags.push_back(oldTag);
    }

    deletedTags.removeDuplicates();

    for (QString word : foundTags)
    {
        bool ignore = false;

        for (QString tag : foundTagsInOldTitle)
        {
            if (tag == word)
            {
                ignore = true;
                break;
            }
        }

        if (!ignore)
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
    }

    for (QString tag : deletedTags)
    {
        for (Trend* trend : *all_trends)
        {
            if (trend->tag == tag.toStdString())
            {
                trend->count--;
            }
        }
    }

    all_trends->sort([](const Trend* a, const Trend* b) { return a->count > b->count; });
}

void PostManager::updateTrendsFromDeletedPostTitle(Post *post)
{
    QString linea = QString::fromStdString(post->title);

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
        for (Trend* trend : *all_trends)
        {
            if (trend->tag == word.toStdString())
            {
                trend->count--;
                break;
            }
        }
    }

    if (foundTags.size() > 0)
    {
        all_trends->sort([](const Trend* a, const Trend* b) { return a->count > b->count; });
    }
}

void PostManager::deletePost(Post *post)
{
    /*
    avl_posts_by_id->remove(post);
    avl_posts_by_authorId->remove(post);
    avl_posts_by_title->remove(post);
    avl_posts_by_pubDate->remove(post);
    avl_posts_by_numLikes->remove(post);
    avl_posts_by_numInteractions->remove(post);
    */
    post->isDeleted = true;

    updateTrendsFromDeletedPostTitle(post);
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

void PostManager::updatePostsAVLs(Post *post)
{
    avl_posts_by_numLikes->remove(post);
    avl_posts_by_numInteractions->remove(post);

    avl_posts_by_numLikes->add(post);
    avl_posts_by_numInteractions->add(post);
}
