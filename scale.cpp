#include "scale.h"
#include <iostream>

using namespace std;

Scale::Scale(int key, bool ismajor)
{
    Ismajor = ismajor;  //if true, the scale is a major scale

    vector<int> major = {2,2,1,2,2,2};  //vector holds #half steps between chord notes for major scale
    vector<int> minor = {2,1,2,2,1,2};  //                                             for minor scales

    Scalenotevals.push_back(key); //Key = rootnote
    if (ismajor)
    {
        for (int i =0; i<6; i++){
            Scalenotevals.push_back(Scalenotevals[i]+major[i]); //build up scale by adding intervals to the preceding note value
        }
    }
    else{
        for (int i =0; i<6; i++){
            Scalenotevals.push_back(Scalenotevals[i]+minor[i]);
        }
    }
}

void Scale::print(){
    cout<<"Major "<<Ismajor<<endl;
    cout<<"Notes "<<endl;
    for(auto note:Scalenotevals){
        cout<<note<<" ";
    }
    cout<<endl;
}
