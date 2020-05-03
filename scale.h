#ifndef SCALE_H
#define SCALE_H
#include<vector>

class Scale
{
    std::vector<int> Scalenotes;
    bool Ismajor;
public:
    Scale(int key, bool ismajor); //build up a major/minor scale using a rootnote ('key')
    int getscalenote(int step)  {return Scalenotes[step];}
    bool getmodus()             {return Ismajor;}
    void print();               //print scale notes and major/minor
};

#endif // SCALE_H
