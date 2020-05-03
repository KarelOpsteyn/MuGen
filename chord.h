#ifndef CHORD_H
#define CHORD_H
#include "note.h"
#include "scale.h"
#include <vector>

class Chord
{
    std::vector<Note> Chordnotes; //0 = Bass, 1= Tenor, 2= Alt, 3= Soprano
public:
    Chord();

    void Setnote(Note& note, int& voice);
    //Chord Addchord(int& newgrade, Scale& scale);
};

#endif // CHORD_H
