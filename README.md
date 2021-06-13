# MuGen
A cpp program made in a Qt environment that primarily generates 'random' music using classical music theory rules.

The program has 2 main functionalities: It can generate entirely 'random' music, or can generate music on top of an existing bass line. the program utilises the MIDI protocol for file io.

for now it is capable of generating music using I, IV, V chords in all keys.

# Usecase, class and sequence diagrams
Note that these are not the cleanest of diagrams, but they do the job.

Usecase diagram:  
![MuGen Class diagram](https://user-images.githubusercontent.com/63557739/121802253-b367e780-cc3b-11eb-93e7-25f0daa58589.png)
	
Class diagram: 
![MuGen Class diagram](https://user-images.githubusercontent.com/63557739/121802294-d0041f80-cc3b-11eb-88e2-c9a187e43d20.png)
	
Sequence diagram: 
![MuGen Sequence diagram](https://user-images.githubusercontent.com/63557739/121802304-dd210e80-cc3b-11eb-9273-ddb6e9257f76.jpg)
	
# Links
some usefull links to learn about the MIDI protocol:

Note numbers: 				https://web.archive.org/web/19990502193435/http://harmonycentral.com/MIDI/Doc/table2.html

Status&Data Bytes: 			https://web.archive.org/web/20000407192000/http://www.harmonycentral.com/MIDI/Doc/table1.html

Specifiction Data summary: 		https://web.archive.org/web/20000411015743/http://www.harmonycentral.com/MIDI/Doc/table4.html

Midi messages: 				https://www.midi.org/articles/about-midi-part-3-midi-messages


The midifile class was not made by me, check it out here:

github: 				https://github.com/craigsapp/midifile
