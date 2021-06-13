#ifndef NOTE_H
#define NOTE_H

class Note  //holds note information
{
    int Value;      //60 = C4 61=C4#
    int Length;     //120 = quarter
    int Velocity;   //80 = forte
    int ScaleStep;  //1,3,5, position of note in the scale relative to bassnote
public:
    Note(int &val, int &len, int &vel, int scstep);
    Note(int &val, int &len);
    Note();

    void setScaleStep(int ScaleStep_) {ScaleStep = ScaleStep_;}

    int getlength() {return Length;}
    int getvalue()  {return Value;}
    int getvelocity() {return Velocity;}
    int getscalestep() {return ScaleStep;}
};

#endif // NOTE_H
