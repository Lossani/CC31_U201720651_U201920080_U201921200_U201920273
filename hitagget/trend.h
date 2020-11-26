#ifndef TREND_H
#define TREND_H

#include <iostream>

using namespace std;

class Trend {

public:
    int count;
    string tag;

    Trend(int count, string tag) : count(count), tag(tag)
    {

    }

    ~Trend()
    {

    }
};

#endif // TREND_H
