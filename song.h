#ifndef SONG_H
#define SONG_H
#include "grade.h"
#include <vector>

class Song
{
    std::vector<Grade> Gradeprogression;
    int* Length;
    int* Key;
    int* Tempo;
    // other attributes
public:
    Song(int len);
    Song();
    ~Song();
    void generate();
    void printgradeprogression();
};

#endif // SONG_H
