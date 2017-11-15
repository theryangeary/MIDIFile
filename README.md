# MIDIFile
C++ Library for interfacing with MIDI files, using Arduino

# Usage
Include MIDIFile.h and SdFat library
```
#include <SdFat.h>
#include MIDIFile.h
```
For drop-in replacement of Arduino SD functions with SdFat functions, declare: 
```
SdFat SD;
```
Create a MIDIFile object by passing the constructor the address of an ifstream containing your MIDI file:
```
fstream midiFile;
midiFile.open("path/to/midi/file.mid");
MIDIFile * myMIDIFile = new MIDIFile(&midiFile);
```
Use methods and members to get information about your MIDI file. Let's print some pertinent information to the conosole:
```
cout << "Header chunk length: " << myMIDIFile->getHeaderChunkLength() << endl;
cout << "File format type: " << myMIDIFile->getFormat() << endl;
cout << "Number of tracks: " << myMIDIFile->getNumberOfTracks() << endl;
cout << "Ticks per quarter note: " << myMIDIFile->getTicksPerQuarterNote() << endl;
```
To play back your MIDI data, you'll want to traverse one or more of the MIDI tracks, pausing between events according to the result of getDeltaT.
```
Track * myTrack = myMIDIFile->getTrack(0);

for (int i = 0; i < myTrack->length; i++) {
    std::vector<MIDIEvent*> eventList = myTrack->MIDIEvents;
    MIDIEvent * event = eventList[i];
    cout << "deltaT: " << event->getDeltaT() << endl;
    cout << "status: " << event->getStatus() << endl;
    cout << "channel: " << event->getChannel() << endl;
    cout << "pitch: " << event->getParam1() << endl;
    cout << "velocity: " << event->getParam2() << endl;
}
```
