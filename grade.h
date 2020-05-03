#ifndef GRADE_H
#define GRADE_H
#include "chord.h"

class Grade
{
    Chord* chord;
    int* grade;
public:
    Grade(int grade_);
    Grade();
    ~Grade();
    Grade addgrade();
    void print();
};

#endif // GRADE_H
