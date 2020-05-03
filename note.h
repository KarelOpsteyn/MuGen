#ifndef NOTE_H
#define NOTE_H

class Note
{
    int* Value;      //60 = C4 61=C4#
    int* Length;     //120 = quarter
    int* Velocity;   //80 = forte
public:
    Note(int& val, int& len, int& vel);
    Note(int& val, int& len);
    ~Note();

    int getlength() {return *Length;}
    int getvalue()  {return *Value;}
};

#endif // NOTE_H
