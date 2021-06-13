#ifndef SCALE_H
#define SCALE_H
#include<vector>

class Scale //Is used to generate and hold a scale
{
    std::vector<int> Scalenotevals; //holds 7 scalenotes
    bool Ismajor;                   //if 1: scale is major, if 0: scale is minor
public:
    Scale(int key, bool ismajor); //build up a major/minor scale using a rootnote ('key')

    int getscalenoteval(int step)   {return Scalenotevals[step-1];}
    bool getmodus()                 {return Ismajor;}
    void print();                   //print scale notes and major/minor
};

#endif // SCALE_H
