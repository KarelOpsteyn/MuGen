#include "note.h"

using namespace std;

Note::Note(int& val, int& len, int& vel){
    Value = new int;
    Length = new int;
    Velocity = new int;

    *Value = val;
    *Length = len;
    *Velocity = vel;
}

Note::Note(int& val, int& len){
    Value = new int;
    Length = new int;
    Velocity = new int;

    *Value = val;
    *Length = len;
    *Velocity = 80;
}

Note::~Note(){
    delete Value;
    delete Length;
    delete Velocity;
}
