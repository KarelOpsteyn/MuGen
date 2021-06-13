#ifndef GRADE_H
#define GRADE_H
#include "note.h"
#include "scale.h"
#include <vector>

struct Voicerange{ int lower; int upper; };

class Grade
{
    std::vector<Note*> Chordnotes; //0 = Bass, 1= Tenor, 2= Alt, 3= Soprano
    int grade;

    //randomizevec<T>
    int checklims(int& val, const Voicerange range);
    void randoctave(int& notevalue);

    void SetIniBasVal(      std::vector<int>& notevals, Scale scale, const std::vector<Voicerange> lims);
    void SetIniTenorVal(    std::vector<int>& notesteps, std::vector<int>& notevals, std::vector<int>& scalenotes, Scale scale,const std::vector<Voicerange> lims);
    void SetIniAltVal(      std::vector<int>& notesteps, std::vector<int>& notevals, std::vector<int>& scalenotes, Scale scale, const std::vector<Voicerange> lims);
    void SetIniSopranoVal(  std::vector<int>& notesteps, std::vector<int>& notevals, std::vector<int>& scalenotes, Scale scale, const std::vector<Voicerange> lims);

    void SetBasVal(         std::vector<int>& notesteps, std::vector<int>& notevals, Scale scale,const std::vector<Voicerange> lims);
    void SetTenorVal(       std::vector<int>& notesteps, std::vector<int>& notevals, std::vector<int>& scalenotes, Scale scale, std::vector<Voicerange>& lims, const int type);
    void SetAltVal(         std::vector<int>& notesteps, std::vector<int>& notevals, std::vector<int>& scalenotes, Scale scale, std::vector<Voicerange>& lims);
    void SetSopranoVal(     std::vector<int>& notesteps, std::vector<int>& notevals, std::vector<int>& scalenotes,Grade prevgrade, Scale scale, std::vector<Voicerange>& lims);

    void Solve7th_to_1st(   Grade prevgrade,  std::vector<int>& notesteps, std::vector<int>& notevals, std::vector<int>& scalenote, std::vector<bool>& dovoice);
    void ParallelsCheck(    Grade prevgrade,  std::vector<int>& notevals);
    void Augment7ths_for_minor_chords( Grade prevgrade, std::vector<int>& notesteps, std::vector<int>& notevals, Scale scale);

public:
    Grade();
    Grade(int grade_);
    Grade(int type, int noteval, int notelength, int velocity, Scale& scale);
    ~Grade();

    Note getnote(int i)     {return *Chordnotes[i];}
    int  getnotecount()     {return Chordnotes.size();}

    void setchord(int type, Scale &scale);                  //used to generate the first chord of the song
    void setchord(int type, Scale& scale, Grade& prevgrade);  //used to generate all subsequent chords
    Grade addgrade();                               //used to generate a new grade (grade int)
    void print();
};

#endif // GRADE_H
