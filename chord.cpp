#include "chord.h"



Chord::Chord()
{
}

/*
void Chord::Setnote(Note& note, int& voice){
    Chordnotes[voice] = note;
}
*/

void Chord::Setchord(int& grade, Scale& scale){
    int notelength = 120;

	// Voice_limiters
    struct Voicerange{ int lower; int upper; };
    Voicerange Baslim{40,64};
    Voicerange Tenorlim{47,69};
    Voicerange Altlim{53,76};
    Voicerange Sopranolim{60,84};

	//set bas
    int basval = scale.getscalenoteval(grade);;

    bool basNotOk{1};
    while (basNotOk){
        if(basval<=Baslim.upper){
            if(basval<=Baslim.lower){
                basval += 12;
            }else{
                basNotOk = 0;
            }
        }else{
            basval -= 12;
        }
    }
    Note bas(basval, notelength);
    Chordnotes.push_back(bas);

	//check possible zelfde noten

	//fill other voices

	//check gelijke beweging
}
