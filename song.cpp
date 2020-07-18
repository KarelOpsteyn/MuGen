#include "song.h"

using namespace std;

Song::Song(int len, int key, bool ismajor, int tempo){
    Length = len;
    Key = key;
    IsMajor = ismajor;
    Tempo = tempo;
}

Song::Song()
{
}

Song::~Song(){
    //for(auto& grade : Gradeprogression){
        //delete grade;
    //}
}


void Song::generate(){
    Scale scale(this->Key, this->IsMajor); //Generate the scale the song will be using
    Grade Tonic(1);     //make a Grade object with grade = 1
    Grade Dominant(5);

    int maxtries1 {10};     //maximum number of tries that the first grade can be reinitialised
    int iteration1 {0};
    bool loopstate1 {true};
    while(loopstate1){
        try{
            Grade firstgrade;
            firstgrade.initialize(scale);
            this->Gradeprogression.push_back(firstgrade);
            for(int i = 0; i< this->Length; i++){
                int maxtries2 {10};     //Tries to generate a next Grade object
                int iteration2 {0};
                bool loopstate2 {true};
                while(loopstate2){
                    try{
                        this->Gradeprogression.push_back(Gradeprogression[i].addgrade());
                        this->Gradeprogression.back().setchord(scale, Gradeprogression[i]);

                        loopstate2 = false;
                        cout<<"chord number " << i+1 << " has generated"<<endl;
                    }catch(Exception_GradeProgression& e){  //catch errors
                        this->Gradeprogression.pop_back();
                        cout<< e.what() <<endl;
                        if(++iteration2 >= maxtries2){        //if it takes too many tries, error is thrown
                            if(i == 0){         //If exception happens at the first chord after sufficient tries.
                                throw Exception_GradeInitialisation();
                            }
                            throw e;
                        }
                    }
                }
            }
            try{    //To end the song, a 5 - 1 progression is used. not complete yet.
                this->Gradeprogression.push_back(Dominant);
                this->Gradeprogression.back().setchord(scale,Gradeprogression[Gradeprogression.size() -2]);
                this->Gradeprogression.push_back(Tonic);
                this->Gradeprogression.back().setchord(scale,Gradeprogression[Gradeprogression.size()-2]);
            }catch(Exception_GradeProgression& e){
                cout<<"euhm"<<endl;
            }
            loopstate1 = false;
        }catch(Exception_GradeInitialisation& e){   //catch exception from line 46
            cout<< e.what() <<endl;
            if(++iteration1 >= maxtries1){
                throw e;
            }
        }
    }
}

void Song::printgradeprogression(){
    for(auto& grade : this->Gradeprogression){
        grade.print();
    }
}

void Song::midiExport(string filename){
    int tpq = 120;

    smf::MidiFile midifile;
    midifile.setTicksPerQuarterNote(tpq);   //set tpq value
    midifile.addTimbre(0,0,0,0);             //track, atick, channel, instrument
    midifile.addTempo(0,0,this->Tempo);

    int timetick = 0;
    for(auto grade : Gradeprogression){ //do for all grades in the song
        for(int i = 0; i<grade.getnotecount();i++){ //for all notes of the chord
            Note note = grade.getnote(i);
            midifile.addNoteOn(0,timetick,0,note.getvalue(), note.getvelocity());
            midifile.addNoteOff(0, timetick + note.getlength(),0,note.getvalue());
            if(i == grade.getnotecount()-1){timetick+=note.getlength();} //add notelength to timetick after all notes of a chord have been written -> homophonic music
        }
    }
    midifile.sortTracks();  // sort tracks chronologically
    midifile.write(filename); //filename
    cout<<"written to "<<filename<<endl;
}
