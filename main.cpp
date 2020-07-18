/*
Dit programma maakt muziek die voldoet aan de klassieke harmonieregels
Klassestructuur: Song   ->  Grade   ->  Note
                        ->  Scale
De Grade klasse bevat het bulk van de generatiecode.
*/
#include <iostream>
#include <vector>
#include "scale.h"
#include "song.h"
#include "grade.h"

using namespace std;


int main()
{
    Song newsong(30,62,true, 80);   //length, key, major, tempo
    newsong.generate();             // generate the song
    //newsong.printgradeprogression();
    newsong.midiExport("test.mid"); //Export the song to midi

    return 0;
}
