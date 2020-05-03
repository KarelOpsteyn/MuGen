#include "song.h"
using namespace std;

Song::Song(int len){
    Length = new int;
    Key = new int;
    Tempo = new int;

    *Length = len;

    vector<Grade> vec(len);
    Gradeprogression = vec;
}

Song::Song()
{
    Length = new int;
    Key = new int;
    Tempo = new int;
}

Song::~Song(){
    delete Length;
    delete Key;
    delete Tempo;
}

void Song::generate(){
    Grade start(1);
    this->Gradeprogression.push_back(start);
    for(int i = 0; i< *Length; i++){
        //Grade newgrade = Gradeprogression[i].addgrade();
        this->Gradeprogression.push_back(this->Gradeprogression[i].addgrade());
    }
}

void Song::printgradeprogression(){
    for(auto& grade : this->Gradeprogression){
        grade.print();
    }
}
