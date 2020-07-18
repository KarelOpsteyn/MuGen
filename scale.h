#ifndef SCALE_H
#define SCALE_H
#include<vector>

class Scale //Is used to generate a scale
{
    std::vector<int> Scalenotevals;
    bool Ismajor;
public:
    Scale(int key, bool ismajor); //build up a major/minor scale using a rootnote ('key')

    int getscalenoteval(int step)   {return Scalenotevals[step-1];}
    bool getmodus()                 {return Ismajor;}
    void print();                   //print scale notes and major/minor
};

#endif // SCALE_H
