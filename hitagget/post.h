#ifndef POST_H
#define POST_H
#include <iostream>
#include <list>
#include "user.h"
#include "publication.h"

using namespace std;

class PostComment : public Publication
{
public:
    int parent_post_id = -1;
};

class Post : public Publication
{
public:
    string title;
    string tag;
};

/*
typedef struct
{
    int id = -1;
    char content[255];
    User author;
    int comments_number = 0;
    vector<PostComment> comments;
} Post;
*/
#endif // POST_H
