#include "note.h"

using namespace std;

Note::Note(int& val, int& len, int& vel, int scstep){
    Value = val;
    Length = len;
    Velocity = vel;
    ScaleStep = scstep;
}

Note::Note(int &val, int &len){
    Value = val;
    Length = len;
    Velocity = 80;
}

Note::Note(){

}
