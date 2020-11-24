#ifndef POST_H
#define POST_H
#include <iostream>
#include <list>
#include <vector>

#include "user.h"
#include "publication.h"

using namespace std;

class PostComment : public Publication
{
public:
    int parentPostId = -1;

    PostComment() { }
    ~PostComment() { }
};

class PostInteraction
{
public:
    int id = -1;
    int authorId = -1;
    int parentPostId = -1;
    string date = "";
    bool shared = false;

    PostInteraction() { }
    ~PostInteraction() { }
};

class Post : public Publication
{
public:
    string title;
    unsigned int numLikes;
    vector<PostComment*> comments;
    vector<PostInteraction*> interactions;
    int numInteractions = 0;

    Post() { }
    ~Post() { }
};

#endif // POST_H
