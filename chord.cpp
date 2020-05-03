#include "chord.h"

Chord::Chord()
{

}

void Chord::Setnote(Note& note, int& voice){
    Chordnotes[voice] = note;
}

/*
Chord Chord::add(int& newgrade){
	// Voice_limiters

	//set bas


	//check possible zelfde noten

	//fill other voices

	//check gelijke beweging
}*/
