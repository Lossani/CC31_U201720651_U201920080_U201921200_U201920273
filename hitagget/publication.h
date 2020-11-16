#ifndef PUBLICATION_H
#define PUBLICATION_H

#include <string>

using namespace std;

class Publication
{
public:
    int id = -1;
    int authorId = -1;
    string content = "";
    string pubDate = "";

    Publication() { }
    ~Publication() { }
};

#endif // PUBLICATION_H
