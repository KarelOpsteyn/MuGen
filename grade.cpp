#include <experimental/random>
#include <iostream>
#include "grade.h"
using namespace std;


//template<class T>
int randomize2(int first, double firstweight, int second,double secondweight){
    firstweight /= (firstweight+secondweight);
    secondweight /= (firstweight+secondweight);
    double rval = experimental::randint(1,100)/100. ;
    if(rval<firstweight){return first;}
    else{return second;}
}

//template<class T>
int randomize3(int first, double firstweight, int second,double secondweight,int third, double thirdweight){
    firstweight /= (firstweight+secondweight+thirdweight);
    secondweight /= (firstweight+secondweight+thirdweight);
    thirdweight /= (firstweight+secondweight+thirdweight);
    double rval = experimental::randint(1,100)/100. ;
    if(rval<firstweight){return first;}
    else if(rval<firstweight+secondweight){return second;}
    else{return third;}
}

Grade::Grade()
{
    grade = new int;
    chord = new Chord;
}

Grade::Grade(int grade_)
{
    grade = new int;
    chord = new Chord;

    *grade = grade_;
}

Grade::~Grade(){
    delete grade;
    delete chord;
}

Grade Grade::addgrade(){
    int newgrade;
    switch(*this->grade){
    case 1: //1 4 5
        newgrade = randomize3(1,1,4,1,5,1);
        break;
    case 4: //1 4 5
        newgrade = randomize3(1,1,4,1,5,1);
        break;
    case 5://1 5
        newgrade = randomize2(1,1,5,1);
        break;
    default:
        newgrade = *this->grade;
    }
    Grade Nextgrade(newgrade); //initialise Grade object with newgrade int
    return Nextgrade;
}

void Grade::print(){
    cout<<" "<<*grade<<endl;
}

