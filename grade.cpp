#include <experimental/random>
#include <iostream>
#include <algorithm>
#include <exception>
#include "grade.h"
using namespace std;

//################## AUXILIARY FUNCTIONS ###################

    //Struct voicerange
struct Voicerange{ int lower; int upper; };

    //select random element from vec with statistical weight according to weightvec, LENGTH of both vecs should be equal
template<class T>
T randomizevec(vector<T>& vec, vector<double> weightvec_){
    // make a weightvec that is normalised
    double totalweight;
    for(auto& weight : weightvec_){totalweight += weight; }
    for(auto& weight : weightvec_){weight /= totalweight; }

    //select random from vec
    double rval = experimental::randint(1,100)/100.;
    totalweight = 0;
    for(unsigned i = 0; i<vec.size();i++){
        totalweight+=weightvec_[i];
        if (rval < totalweight){
            return vec[i];
        }
    }
    return vec.back(); //does not get used
}


    //set note within voice limits
int checklims(int& val, const Voicerange range){
    bool limNotOk{1};
    int emergstop = 0;
    while (limNotOk){
        if(val<=range.upper){
            if(val<range.lower){
                val += 12;
            }else{
                limNotOk = 0;
            }
        }else{
            val -= 12;
        }
        emergstop++;
        if(emergstop>100){
            throw Exception_NoteLimiter();
            return val;
        }
    }
    return val;
}


    //jump noteval one octave up or down
void randoctave(int& notevalue){
    double rval = experimental::randint(1,100)/100.;
    if(rval>0.5){
        notevalue+=12;
    }else{
        notevalue-=12;
    }
}



//################## CONSTRUCTORS ###########################

Grade::Grade()
{
}


Grade::Grade(int grade_)
{
    grade = grade_;
}

Grade::~Grade(){
    /*for(int i = 0; i< Chordnotes.size(); i++ ){
        //delete Chordnotes[i];
    }
    cout<<"Destructor called"<<endl;
*/
}



//################## MEMBER FUNCTIONS ########################

    //returns grade object with grade member
Grade Grade::addgrade(){
    int newgrade;
    vector<int> gradevec;
    vector<double> weightvec;
    switch(this->grade){
    case 1: //1 4 5
        gradevec = {1,4,5};
        weightvec= {1,2,2};
        newgrade = randomizevec<int>(gradevec, weightvec);
        break;
    case 4: //1 4 5
        gradevec = {1,4,5};
        weightvec= {2,1,3};
        newgrade = randomizevec<int>(gradevec, weightvec);
        break;
    case 5: //1 5
        gradevec = {1,5};
        weightvec= {2,1};
        newgrade = randomizevec<int>(gradevec, weightvec);
        break;
    default:
        newgrade = this->grade;
    }
    Grade Nextgrade(newgrade); //initialise Grade object with newgrade int
    return Nextgrade;
}


void Grade::initialize(Scale &scale){ //used to generate the first chord of the song
    this->grade = 1;

    int notelength = 120;

    // Voice_limiters
    Voicerange Baslim{40,64}; //40,64
    Voicerange Tenorlim{47,69};
    Voicerange Altlim{53,76};
    Voicerange Sopranolim{60,84};

    //set basval
    int basval = scale.getscalenoteval(grade);
    checklims(basval, Baslim);  //get note within voice limits
    randoctave(basval);         //transpose an octave up/down (random)
    checklims(basval, Baslim);  //get note within voice limits

    //set tenorval
    vector<int> scalenotes{1,3,5};                                                          //possible notes from scale that can be used
    vector<double> weights{1,1,1};                                                          //stat. weight corresponding to the chance the above
    int tenorstep = randomizevec(scalenotes,weights);                                       //choose one of scalenotes
    int tenorval = basval + scale.getscalenoteval(tenorstep) - scale.getscalenoteval(1);    //set tenor note value
    checklims(tenorval, Tenorlim);                                                          //adjust note value to be within voice limits

    int emergstop{0};                                   //if while loop takes too long this causes a stop.
    while(tenorval <= basval && emergstop<20){          //tenor noteval should be above bas note val
        randoctave(basval);
        checklims(basval,Baslim);
        randoctave(tenorval);
        checklims(tenorval, Tenorlim);
        emergstop++;
    }

    //set altval
    scalenotes.erase(std::remove(scalenotes.begin(), scalenotes.end(), tenorstep), scalenotes.end());   //remove scalenotes element used by tenor.
    weights.pop_back();
    int altstep = randomizevec(scalenotes,weights);
    int altval = basval + scale.getscalenoteval(altstep) - scale.getscalenoteval(1);
    checklims(altval, Altlim);

    emergstop = 0;
    while(altval <= tenorval && emergstop<20){          //alt noteval should be above tenor note val
        randoctave(altval);
        checklims(altval,Altlim);
        emergstop++;
    }

    //set sopranoval
    scalenotes.erase(remove(scalenotes.begin(), scalenotes.end(), altstep));
    weights.pop_back();
    int sopranostep = scalenotes[0];
    int sopranoval = basval + scale.getscalenoteval(scalenotes[0]) - scale.getscalenoteval(1);
    checklims(sopranoval, Sopranolim);

    emergstop = 0;
    while(sopranoval <= altval && emergstop<20){          //soprano noteval should be above alt note val
        randoctave(sopranoval);
        checklims(sopranoval,Sopranolim);
        emergstop++;
    }

    //print notes
    //cout<<basval<<' '<<tenorval<<' '<<altval<<' '<<sopranoval<<endl;

    //Set notes
    int velocity = 80;
    Chordnotes.push_back(new Note(basval, notelength, velocity, 1));
    Chordnotes.push_back(new Note(tenorval, notelength, velocity, tenorstep));
    Chordnotes.push_back(new Note(altval, notelength, velocity, altstep));
    Chordnotes.push_back(new Note(sopranoval, notelength, velocity, sopranostep));
}


void Grade::setchord(Scale& scale, Grade& prevgrade){ //used to generate all subsequent chords. partly the same as initialize
    int notelength = 120;
    int velocity = 80;

    // Voice_limiters: adapted to prevent voice crossing, human limitations, and prevent jumps greater then 8va/8vb
    Voicerange Baslim{      max(40, prevgrade.getnote(0).getvalue()-12),
                            min(64, prevgrade.getnote(0).getvalue()+12)};
    Voicerange Tenorlim{    max(prevgrade.getnote(0).getvalue(),max(47, prevgrade.getnote(1).getvalue()-12)),
                            min(prevgrade.getnote(2).getvalue(),min(69, prevgrade.getnote(1).getvalue()+12))};
    Voicerange Altlim{      max(prevgrade.getnote(1).getvalue(),max(53, prevgrade.getnote(2).getvalue()-12)),
                            min(prevgrade.getnote(3).getvalue(),min(76, prevgrade.getnote(2).getvalue()+12))};
    Voicerange Sopranolim{  max(prevgrade.getnote(2).getvalue(),max(60, prevgrade.getnote(3).getvalue()-12)),
                            min(84, prevgrade.getnote(3).getvalue()+12)};

    int maxtries {15};
    int iteration {0};
    bool loopstate {true};
    while(loopstate){
        try{
        //intialize variables
            vector<int> scalenotes{1,3,5};      //possible notes from scale that can be used

            vector<int> notevals(4);
            vector<int> notesteps(4);
            vector<bool> dovoice = {true,true,true,true};

        //set basval
            notesteps[0] = 1;
            notevals[0] = scale.getscalenoteval(grade);
            checklims(notevals[0], Baslim);  //get note within voice limits
            randoctave(notevals[0]);         //transpose an octave up/down (random)
            checklims(notevals[0], Baslim);  //get note within voice limits

        //Solving 7th step into 1st
            if(prevgrade.grade == 5){
                for(int i = 1; i<4; i++){
                    if((prevgrade.getnote(i).getscalestep()) == 3){

                        if(grade == 1){
                            notesteps[i] = 1;
                            notevals[i] = prevgrade.getnote(i).getvalue()+1;
                        }
                        else if(grade == 4) {
                            notesteps[i] = 5;
                            notevals[i] = prevgrade.getnote(i).getvalue()+1;
                        }
                        else if(grade == 5){
                            notesteps[i] = 3;
                            notevals[i] = prevgrade.getnote(i).getvalue();     //MAG EVT VAN OCTAAF WISSELEN
                        }
                        dovoice[i] = false;
                        scalenotes.erase(std::remove(scalenotes.begin(), scalenotes.end(), notesteps[i]), scalenotes.end());
                    }
                }
            }

        //set sopranoval
            if(dovoice[3]){
                //search for step closest to previous soprano notevalue:
                int prevsopval = prevgrade.getnote(3).getvalue();
                //bring prevsopval in the scalenotes range;
                while (prevsopval > scalenotes.back() && prevsopval>scalenotes[0]) {
                    if(prevsopval< scalenotes.back()){
                        if(prevsopval < scalenotes[0]){
                            prevsopval += 12;
                        }
                    }else{
                        prevsopval -= 12;
                    }
                }
                //compare previous sopranoval with possible following notes.
                vector<int> dist {0,0,0};
                for(int i = 0; i<3; i++){
                    dist[i] = std::abs(scalenotes[i]-prevsopval);
                }
                //invert distances to create weightvector:
                vector<double> distweight(3);
                for(int i = 0; i<3;i++){
                    if(dist[i] != 0){
                        distweight[i] = 1./dist[i];
                    }else{
                        distweight[i] = 0.25;
                    }
                }
                //get sopranovalue and set it in the right octave
                notesteps[3] = randomizevec(scalenotes,distweight);
                scalenotes.erase(std::remove(scalenotes.begin(), scalenotes.end(), notesteps[3]), scalenotes.end());
                notevals[3] = notevals[0] + scale.getscalenoteval(notesteps[3]) - scale.getscalenoteval(1);
                checklims(notevals[3], Sopranolim);
                //if jump is too big, take closer octave
                if(notevals[3]-prevgrade.getnote(3).getvalue() > 9){ //VALUE == 9?
                    notevals[3] -=12;
                }else if(notevals[3] - prevgrade.getnote(3).getvalue() < 7){
                    notevals[3] +=12;
                }
                checklims(notevals[3], Sopranolim);
            }
        //set altval

            if(dovoice[2]){
                vector<double> weights{1,1};

                Altlim.upper = min(Altlim.upper,notevals[3]);       //adjust voicelimiter to include soprano
                Altlim.lower = max(Altlim.lower, notevals[3] - 12); // Alt is maximum 8vb wrt soprano

                notesteps[2] = randomizevec(scalenotes,weights);    //get random scalenote
                scalenotes.erase(std::remove(scalenotes.begin(), scalenotes.end(), notesteps[2]), scalenotes.end());

                notevals[2] = notevals[0] + scale.getscalenoteval(notesteps[2]) - scale.getscalenoteval(1);    //set scalenote in scale
                checklims(notevals[2], Altlim);
            }
        //set tenorval
            if(dovoice[1]){
                Tenorlim.upper = min(notevals[2], Tenorlim.upper);      //adjust voicelimiter to include alt
                Tenorlim.lower = max(Tenorlim.lower, notevals[2] -12);  //Tenor is max. 8vb wrt Alt

                notesteps[1] = scalenotes[0];                                       //choose remaining scalenote
                notevals[1] = notevals[0] + scale.getscalenoteval(notesteps[1]) - scale.getscalenoteval(1);      //set tenor note value
                checklims(notevals[1], Tenorlim);                                                               //adjust note value to be within voice limits

                int emergstop{0};                                           //if while loop takes too long this causes a stop.
                while(notevals[1] < notevals[0] && emergstop<20){           //tenor noteval should be above bas note val
                    randoctave(notevals[0]);
                    checklims(notevals[0], Baslim);
                    randoctave(notevals[1]);
                    checklims(notevals[1], Tenorlim);
                    emergstop++;
                }
            }

        //Check for parallel 5ths, 8ths
            if(prevgrade.grade != grade){
                int gap;
                int prevgap;
                for(int i = 0; i<3;i++){
                    for(int j = i+1; j<4; j++){
                        gap = notevals[j] - notevals[i];
                        prevgap = prevgrade.getnote(j).getvalue() - prevgrade.getnote(i).getvalue();
                        if((gap == 7 || gap == 12) && gap == prevgap){  //if  (5th (7) or 8th (12)) and gaps match
                            throw Exception_NoteLimiter();
                        }
                    }
                }
            }
        //minor scales: augmented 7th for 3rd,5th,7th grade:
            if(!scale.getmodus()){
                if(grade == 5 && prevgrade.grade != 5){ //prevgrade == 5 is dealt with when solving 7ths to 1s
                    for(int i = 1; i<4;i++){
                        if(notesteps[i] == 3){
                            notevals[i] += 1;
                        }
                    }
                }
            }

        //Set notes
            /*
            Note bas(notevals[0], notelength, velocity, notesteps[0]);
            Chordnotes.push_back(bas);
            Note tenor(notevals[1], notelength, velocity, notesteps[1]);
            Chordnotes.push_back(tenor);
            Note alt(notevals[2], notelength, velocity, notesteps[2]);
            Chordnotes.push_back(alt);
            Note sopr(notevals[3], notelength, velocity, notesteps[3]);
            Chordnotes.push_back(sopr);
            */
            for(int i = 0; i<4;i++){
                Chordnotes.emplace_back(new Note(notevals[i], notelength, velocity, notesteps[i]));
            }

           loopstate = false;      //break out loop if no error occurs

        }catch(Exception_NoteLimiter& e){     //catch errors
            cout<< e.what() <<endl;
            if(++iteration >= maxtries){    //if it takes too many tries higher level error is thrown
                //remove grade element, try another grade
                throw Exception_GradeProgression();
            }
        }
    }

    //print notes
    //cout<<basval<<' '<<tenorval<<' '<<altval<<' '<<sopranoval<<endl;
}


void Grade::print(){
    cout<<" "<<grade<<' '<<(*Chordnotes[0]).getvalue()<<' '<<(*Chordnotes[1]).getvalue()<<' '<<(*Chordnotes[2]).getvalue()<<' '<<(*Chordnotes[3]).getvalue()<<endl;
}

