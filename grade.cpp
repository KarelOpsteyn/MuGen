#include <experimental/random>
#include <iostream>
#include <algorithm>
#include <exception>
#include "grade.h"
#include "exception.h"
using namespace std;

//################## AUXILIARY FUNCTIONS ###################

    //select random element from vec with statistical weight according to weightvec, LENGTH of both vecs should be equal
template<class T>
T randomizevec(vector<T>& vec, vector<double> weightvec_){
    // make a weightvec that is normalised
    double totalweight = 0;
    for(auto& weight : weightvec_){totalweight += weight; }
    for(auto& weight : weightvec_){weight /= totalweight; }

    //select random from vec
    double rval = experimental::randint(1,100)/100.;    //random value
    totalweight = 0;
    for(unsigned i = 0; i<vec.size();i++){
        totalweight+=weightvec_[i];
        if (rval < totalweight){
            return vec[i];                              //when random val < smaller than total weight
        }
    }
    return vec.back(); //does not get used
}

    //set note within voice limits by transposing it 8va or 8vb
int Grade::checklims(int& val, const Voicerange range){ //in: notevalue, voicerange
    bool limNotOk = true;   //assume value is not within limits
    int emergstop = 0;      //te break while in emergency
    while (limNotOk){
        if(val<=range.upper){
            if(val<range.lower){
                val += 12;          //if val smaller than lower range 8va
            }else{
                limNotOk = 0;       //if val ok then break
            }
        }else{
            val -= 12;              //if val too high 8vb
        }
        emergstop++;
        if(emergstop>100){          //if loop takes too many iterations throw exception (catch in setchord(...))
            throw Exception_NoteLimiter();
            return val;
        }
    }
    return val;
}

    //jump noteval one octave up or down
void Grade::randoctave(int& notevalue){
    double rval = experimental::randint(1,100)/100.;
    if(rval>0.5){
        notevalue+=12;
    }else{
        notevalue-=12;
    }
}

    //return one of possible notelength values
int getnotelength(){
    vector<int> notelengths{120,240};
    vector<double> weight{1,0.5};
    return randomizevec(notelengths, weight);  // when enabled: random note lengths
    //return 120;                              //when  enabled: only quarter notes
}

void Grade::SetIniBasVal(vector<int>& notevals, Scale scale, const vector<Voicerange> lims){
    notevals[0] = scale.getscalenoteval(this->grade);
    checklims(notevals[0], lims[0]);  //get note within voice limits
    randoctave(notevals[0]);         //transpose an octave up/down (random)
    checklims(notevals[0], lims[0]);  //get note within voice limits
}

void Grade::SetIniSopranoVal(vector<int>& notesteps, vector<int>& notevals, vector<int>& scalenotes, Scale scale, const vector<Voicerange> lims){
    notesteps[3] = scalenotes[0];
    notevals[3] = notevals[0] + scale.getscalenoteval(notesteps[3]) - scale.getscalenoteval(1);
    checklims(notevals[3], lims[3]);

    int emergstop = 0;
    while(notevals[3] <= notevals[2] && emergstop<20){          //soprano noteval should be above alt note val
        randoctave(notevals[3]);
        checklims(notevals[3], lims[3]);
        emergstop++;
    }
}

void Grade::SetIniAltVal(vector<int>& notesteps, vector<int>& notevals, vector<int>& scalenotes, Scale scale, const vector<Voicerange> lims){
    vector<double> weights;
    for(unsigned i = 0;i<scalenotes.size();i++){weights.push_back(1);}   //make weigths vector according to size of scalenotes

    notesteps[2] = randomizevec(scalenotes,weights);                            //choose one of scalenotes
    scalenotes.erase(remove(scalenotes.begin(), scalenotes.end(), notesteps[2]));    //remove scalenotes element used by alt.

    notevals[2] = notevals[0] + scale.getscalenoteval(notesteps[2]) - scale.getscalenoteval(1);
    checklims(notevals[2], lims[2]);

    int emergstop = 0;
    while(notevals[2] <= notevals[1] && emergstop<20){          //alt noteval should be above tenor note val
        randoctave(notevals[2]);
        checklims(notevals[2],lims[2]);
        emergstop++;
    }
}

void Grade::SetIniTenorVal(vector<int>& notesteps, vector<int>& notevals, vector<int>& scalenotes, Scale scale,const vector<Voicerange> lims){
    vector<double> weights;
    for(unsigned i = 0;i<scalenotes.size();i++){weights.push_back(1);}   //make weigths vector according to size of scalenotes

    notesteps[1] = randomizevec(scalenotes,weights);                                                        //choose one of scalenotes
    scalenotes.erase(std::remove(scalenotes.begin(), scalenotes.end(), notesteps[1]), scalenotes.end());    //remove scalenotes element used by tenor.

    notevals[1] = notevals[0] + scale.getscalenoteval(notesteps[1]) - scale.getscalenoteval(1); //set tenor note value
    checklims(notevals[1], lims[1]);                                                           //adjust note value to be within voice limits

    int emergstop = 0;                                   //if while loop takes too long this causes a stop.
    while(notevals[1]  <= notevals[0]  && emergstop<20){          //tenor noteval should be above bas note val
        randoctave(notevals[0]);
        checklims(notevals[0], lims[0]);
        randoctave(notevals[1]);
        checklims(notevals[1], lims[1]);
        emergstop++;
    }
}

void Grade::SetBasVal(vector<int>& notesteps, vector<int>& notevals, Scale scale,const vector<Voicerange> lims){
    notesteps[0] = 1;
    notevals[0] = scale.getscalenoteval(this->grade);
    checklims(notevals[0], lims[0]);    //get note within voice limits
    randoctave(notevals[0]);            //transpose an octave up/down (random)
    checklims(notevals[0], lims[0]);    //get note within voice limits
}

void Grade::SetTenorVal(vector<int>& notesteps, vector<int>& notevals, vector<int>& scalenotes, Scale scale,vector<Voicerange>& lims, const int type){
    lims[1].upper = min(notevals[2], lims[1].upper);      //adjust voicelimiter to include alt
    lims[1].lower = max(lims[1].lower, notevals[2] -12);  //Tenor is max. 8vb wrt Alt

    notesteps[1] = scalenotes[0];                                       //choose remaining scalenote
    notevals[1] = notevals[0] + scale.getscalenoteval(notesteps[1]) - scale.getscalenoteval(1);      //set tenor note value
    checklims(notevals[1], lims[1]);                                                               //adjust note value to be within voice limits

    int emergstop{0};                                           //if while loop takes too long this causes a stop.
    while(notevals[1] < notevals[0] && emergstop<20){           //tenor noteval should be above bas note val
        if(type != 1){
            randoctave(notevals[0]);
            checklims(notevals[0], lims[0]);
        }
        randoctave(notevals[1]);
        checklims(notevals[1], lims[1]);
        emergstop++;
    }
}

void Grade::SetAltVal(vector<int>& notesteps, vector<int>& notevals, vector<int>& scalenotes, Scale scale,vector<Voicerange>& lims){
    vector<double> weights{1,1};

    lims[2].upper = min(lims[2].upper,notevals[3]);       //adjust voicelimiter to include soprano
    lims[2].lower = max(lims[2].lower, notevals[3] - 12); // Alt is maximum 8vb wrt soprano

    notesteps[2] = randomizevec(scalenotes,weights);    //get random scalenote
    scalenotes.erase(std::remove(scalenotes.begin(), scalenotes.end(), notesteps[2]), scalenotes.end());

    notevals[2] = notevals[0] + scale.getscalenoteval(notesteps[2]) - scale.getscalenoteval(1);    //set scalenote in scale
    checklims(notevals[2], lims[2]);
}

void Grade::SetSopranoVal(vector<int>& notesteps, vector<int>& notevals, vector<int>& scalenotes,Grade prevgrade, Scale scale,vector<Voicerange>& lims){
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
    checklims(notevals[3], lims[3]);
    //if jump is too big, take closer octave
    if(notevals[3]-prevgrade.getnote(3).getvalue() > 9){ //VALUE == 9?
        notevals[3] -=12;
    }else if(notevals[3] - prevgrade.getnote(3).getvalue() < 7){
        notevals[3] +=12;
    }
    checklims(notevals[3], lims[3]);
}

void Grade::Solve7th_to_1st(Grade prevgrade,  vector<int>& notesteps, vector<int>& notevals, vector<int>& scalenotes, vector<bool>& dovoice){
    if(prevgrade.grade == 5){
        for(int i = 1; i<4; i++){
            if((prevgrade.getnote(i).getscalestep()) == 3){
                if(grade == 1){
                    notesteps[i] = 1;
                    notevals[i] = prevgrade.getnote(i).getvalue()+1;
                }
                else if(grade == 5){
                    notesteps[i] = 3;
                    notevals[i] = prevgrade.getnote(i).getvalue();
                }
                dovoice[i] = false;
                scalenotes.erase(std::remove(scalenotes.begin(), scalenotes.end(), notesteps[i]), scalenotes.end());
            }
        }
    }
}

void Grade::ParallelsCheck(Grade prevgrade,  vector<int>& notevals){
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
}

void Grade::Augment7ths_for_minor_chords(Grade prevgrade, std::vector<int>& notesteps, std::vector<int>& notevals, Scale scale){
    if(!scale.getmodus()){
        if(grade == 5 && prevgrade.grade != 5){ //prevgrade == 5 is dealt with when solving 7ths to 1s
            for(int i = 1; i<4;i++){
                if(notesteps[i] == 3){
                    notevals[i] += 1;
                }
            }
        }
    }
}

//################## CONSTRUCTORS ###########################

Grade::Grade()
{
    Chordnotes = std::vector<Note*>(4);
}

Grade::Grade(int grade_)
{
    Chordnotes = std::vector<Note*>(4);
    grade = grade_;
}

Grade::Grade(int type, int noteval, int notelength, int velocity, Scale& scale){
    Chordnotes = std::vector<Note*>(4);

    if(type == 0){  //Signifies a bassvalue
        for(int i =0; i<7; i++){
            if(noteval%12 == scale.getscalenoteval(i)%12){
                grade = i;
            }
        }
        Chordnotes[0] = new Note(noteval, notelength, velocity, 1); //scalestep of bass is always 1 (unless 146)
    }
}

Grade::~Grade(){
   for(unsigned i = 0; i< Chordnotes.size(); i++ ){
        if(!Chordnotes[i]){delete Chordnotes[i];}   //if not nullptr call destructor
    }
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


void Grade::setchord(int type, Scale &scale){ //used to generate the first chord of the song
    //type 0: all voices are generated
    //type 1: bas is preloaded, does not need to be generated.

    if(type == 0){
        this->grade = 1;
    }

    int velocity = 80;
    int notelength = 120;
    if(type == 1){
        notelength = this->getnote(0).getlength();
        velocity = this->getnote(0).getvelocity();
    }

    // Voice_limiters
    vector<Voicerange> lims{{40,64},{47,69},{53,76},{60,84}};

    int maxtries {15};
    int iteration {0};
    bool loopstate {true};
    while(loopstate){
        try{

            //initialize variables
            vector<int> scalenotes{1,3,5};      //possible notes from scale that can be used

            vector<int> notevals(4);
            vector<int> notesteps(4);
            vector<bool> dovoice = {true,true,true,true};

            if(type == 1){
                dovoice[0] = false;
                notevals[0] = this->getnote(0).getvalue();
                notesteps[0] = this->getnote(0).getscalestep();
            }

            //set basval
            if(dovoice[0]){
                SetIniBasVal(notevals, scale, lims);
            }

            //set tenorval
            if(dovoice[1]){
                SetIniTenorVal(notesteps, notevals, scalenotes, scale, lims);
            }

            //set altval
            if(dovoice[2]){
                SetIniAltVal(notesteps, notevals, scalenotes, scale, lims);
            }

            //set sopranoval
            if(dovoice[3]){
                SetIniSopranoVal(notesteps, notevals, scalenotes, scale, lims);
            }

            //Set notes
            for(int i = 0; i<4;i++){
                if(type == 1 && i == 0){} //do not write bas as this is already set in type == 1
                else{
                    Chordnotes[i] = new Note(notevals[i], notelength, velocity, notesteps[i]);
                }
            }

            loopstate = false;      //break out loop if no error occurs

        }catch(Exception_NoteLimiter& e){     //catch errors
            cout<< e.what() <<endl;
            cout<<"First chord generation took too many tries"<<endl;
            if(++iteration >= maxtries){    //if it takes too many tries higher level error is thrown
                throw Exception_GradeInitialisation();
            }
        }
    }
}


void Grade::setchord(int type, Scale& scale, Grade& prevgrade){ //used to generate all subsequent chords. partly the same as initialize
    //type 0: all voices are generated
    //type 1: bas is preloaded, does not need to be generated.

    int notelength = getnotelength();
    int velocity = 80;

    if(type ==1){
        notelength = this->getnote(0).getlength();
        velocity = this->getnote(0).getvelocity();
    }

    // Voice_limiters: adapted to prevent voice crossing, human limitations, and prevent jumps greater then 8va/8vb
    vector<Voicerange> lims{
                            {max(40, prevgrade.getnote(0).getvalue()-12),   //bas
                            min(64, prevgrade.getnote(0).getvalue()+12)},
                            {max(prevgrade.getnote(0).getvalue(),max(47, prevgrade.getnote(1).getvalue()-12)),  //tenor
                            min(prevgrade.getnote(2).getvalue(),min(69, prevgrade.getnote(1).getvalue()+12))},
                            {max(prevgrade.getnote(1).getvalue(),max(53, prevgrade.getnote(2).getvalue()-12)),  //alt
                            min(prevgrade.getnote(3).getvalue(),min(76, prevgrade.getnote(2).getvalue()+12))},
                            {max(prevgrade.getnote(2).getvalue(),max(60, prevgrade.getnote(3).getvalue()-12)),  //soprano
                            min(84, prevgrade.getnote(3).getvalue()+12)}};

    int maxtries {15};
    int iteration {0};
    bool loopstate {true};
    while(loopstate){
        try{
        //initialize variables
            vector<int> scalenotes{1,3,5};      //possible notes from scale that can be used

            vector<int> notevals(4);
            vector<int> notesteps(4);
            vector<bool> dovoice = {true,true,true,true};
            if(type == 1){
                dovoice[0] = false;
                notevals[0] = this->getnote(0).getvalue();
                notesteps[0] = this->getnote(0).getscalestep();
            }

        //set basval
            if(dovoice[0]){
                SetBasVal(notesteps, notevals, scale, lims);
            }
        //Solving 7th step into 1st
            Solve7th_to_1st(prevgrade, notesteps, notevals, scalenotes, dovoice);

        //set sopranoval
            if(dovoice[3]){
                SetSopranoVal(  notesteps, notevals, scalenotes, prevgrade, scale, lims);
            }

        //set altval

            if(dovoice[2]){
                SetAltVal(      notesteps, notevals, scalenotes, scale, lims);
            }

        //set tenorval
            if(dovoice[1]){
                SetTenorVal(    notesteps, notevals, scalenotes, scale, lims, type);
            }

        //Check for parallel 5ths, 8ths
            ParallelsCheck(prevgrade, notevals);

        //minor scales: augmented 7th for 3rd,5th,7th grade:
            Augment7ths_for_minor_chords(prevgrade, notesteps, notevals, scale);

        //Set notes
            for(int i = 0; i<4;i++){
                if(type == 1 && i == 0){}
                else{
                    Chordnotes[i] = new Note(notevals[i], notelength, velocity, notesteps[i]);
                }
            }

           loopstate = false;      //break out loop if no error occurs

        }catch(Exception_NoteLimiter& e){     //catch errors
            cout<< e.what() <<endl;
            if(++iteration >= maxtries){    //if it takes too many tries higher level error is thrown
                //remove grade element, try another grade (see song.cpp, generate() and  solve_bassproblem())
                throw Exception_GradeProgression();
            }
        }
    }
}


void Grade::print(){
    cout<<" "<<grade<<' '<<(*Chordnotes[0]).getvalue()<<' '<<(*Chordnotes[1]).getvalue()<<' '<<(*Chordnotes[2]).getvalue()<<' '<<(*Chordnotes[3]).getvalue()<<endl;
}

