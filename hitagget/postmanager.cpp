#include <iostream>
#include "postmanager.h"
#include <fstream>
#include <QMessageBox>
#include <ctime>

using namespace std;

PostManager::PostManager() : ListController<Post*, int, int>(
    [](Post* post)
    {
        return post->id;
    },
    [](int value1, int value2)
    {
        return value1 == value2;
    },
    [](ofstream& file, Post* element)
    {
    //QMessageBox msg;
        //msg.setText("newPost->get_post_content()");
        //msg.exec();
        file << element->id << endl;
        file << element->authorId << endl;
        file << element->content << endl;
        file << element->title << endl;
    },
    [](ifstream& file)
    {
        list<Post*> retrievedElements;

        Post* currentPost;

        string id, authorId, numLikes;

        getline(file, id); // Ignoring first line, temporarily using id for getline parameter

        while (getline(file, id, '\t'))
        {
            currentPost = new Post();

            getline(file, authorId, '\t');
            getline(file, currentPost->title, '\t');
            getline(file, currentPost->content, '\t');
            getline(file, currentPost->pubDate, '\t');
            getline(file, numLikes);

            currentPost->id = stoi(id);
            currentPost->authorId = stoi(authorId);
            currentPost->numLikes = stoi(numLikes);

            retrievedElements.push_front(currentPost);
        }

        return retrievedElements;
    },
    "publications.tsv")
{

}

PostManager::~PostManager()
{

}

/*
void PostManager::add_post(Post post)
{
    add_element(post);
}
*/

void PostManager::addPost(int authorId, string title, string content)
{
    Post* newPost;

    newPost->id = time(0);
    newPost->authorId = authorId;
    newPost->content = content;
    newPost->title = title;
    time_t now = time(0);
    newPost->pubDate = ctime(&now);
    newPost->pubDate = newPost->pubDate.erase(newPost->pubDate.find_last_not_of("\t\n\v\f\r ") + 1);
    add_element(newPost);
}

void PostManager::addPost(Post* post)
{
    post->id = time(0);
    add_element(post);
}

void PostManager::updatePost(Post* post)
{
    update_element(post);
}

void PostManager::deletePost(int id)
{
    delete_element(id);
}

list<Post*> PostManager::getAuthorPosts(int userId)
{
    function<int(Post*)> field_comparator = [](Post* post)
    {
        return post->authorId;
    };

    return find_elements(field_comparator, userId);
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
    return get_all_elements();
}

Post* PostManager::getPost(int postId)
{
    return get_element(postId);
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
