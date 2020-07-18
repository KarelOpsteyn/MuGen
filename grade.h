#ifndef GRADE_H
#define GRADE_H
#include "note.h"
#include "scale.h"
#include <vector>

//// EXCEPTIONS ////

//Exceptions are used to check if e.g. a random new chord adheres to the set rules in setchord().

struct Exception_NoteLimiter : public std::exception
{
    const char * what () const throw ()
    {
        return "Chord generation ran into an issue";
    }
};

struct Exception_GradeProgression : public std::exception
{
    const char * what () const throw ()
    {
        return "Grade progression could not be resolved";
    }
};

struct Exception_GradeInitialisation : public std::exception
{
    const char * what () const throw ()
    {
        return  "Bad First Grade";
    }
};

//// CLASS ////
class Grade
{
    std::vector<Note*> Chordnotes; //0 = Bass, 1= Tenor, 2= Alt, 3= Soprano
    int grade;
public:
    Grade();
    Grade(int grade_);
    ~Grade();

    Note getnote(int i)     {return *Chordnotes[i];}
    int  getnotecount()     {return Chordnotes.size();}

    void initialize(Scale& scale);                  //used to generate the first chord of the song
    void setchord(Scale& scale, Grade& prevgrade);  //used to generate all subsequent chords
    Grade addgrade();                               //used to generate a new grade (grade int)
    void print();
};

#endif // GRADE_H
