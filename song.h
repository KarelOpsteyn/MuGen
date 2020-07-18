#ifndef SONG_H
#define SONG_H
#include "Dependencies/MidiFile.h"
#include "Dependencies/Options.h"
#include "grade.h"
#include "scale.h"
#include <vector>
#include <string>

class Song
{
    std::vector<Grade> Gradeprogression;    //vector that holds all grades (chords)
    int Length;     //defines the approximate length of the song
    int Key;        //defines the key the song is written in
    bool IsMajor;   //defines if the key is major or minor
    int Tempo;      //defines the tempo of the song
    // other attributes
public:
    Song(int len, int key, bool ismajor, int tempo);    //initialise a song object with some settings
    Song();
    ~Song();

    void generate();                        //generate the song
    void printgradeprogression();           //print grade progression and chords in CMD
    void midiExport(std::string filename);  //export to midi
};

#endif // SONG_H
