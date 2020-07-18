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

    //void Setnote(Note& note, int& voice);
    void Setchord(int& grade, Scale& scale);
    Note getbas()   {return Chordnotes[0];};
    //bool Checkchord(Chord& prevch);
};

#endif // CHORD_H
