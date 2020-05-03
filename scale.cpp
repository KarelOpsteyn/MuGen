#include "scale.h"
#include <iostream>

using namespace std;

Scale::Scale(int key, bool ismajor)
{
    Ismajor = ismajor;

    vector<int> major = {2,2,1,2,2,2};
    vector<int> minor = {2,1,2,2,1,2};

    Scalenotes.push_back(key); //Key = rootnote
    if (ismajor)
    {
        for (int i =0; i<6; i++){
            Scalenotes.push_back(Scalenotes[i]+major[i]); //build up scale by adding intervals
        }
    }
    else{
        for (int i =0; i<6; i++){
            Scalenotes.push_back(Scalenotes[i]+minor[i]);
        }
    }
}



void Scale::print(){
    cout<<"Major "<<Ismajor<<endl;
    cout<<"Notes "<<endl;
    for(auto note:Scalenotes){
        cout<<note<<" ";
    }
    cout<<endl;
}
