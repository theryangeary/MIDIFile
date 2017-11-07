#include "MIDIFile.h"
#include <iostream>
#include <fstream>
#include <typeinfo>

using namespace::std;

int main() {
	ifstream midiFile;
	midiFile.open("emajor.mid");
	char tmp;
	int x;
//	while (midiFile.get(tmp)){
	//	x = (int) tmp;
	//		cout << std::hex << x <<endl;
	//}
	MIDIFile * emajor = new MIDIFile(&midiFile);
	cout << "Header chunk length: " << emajor->getHeaderChunkLength() << endl;
	cout << "File format type: " << emajor->getFormat() << endl;
	cout << "Number of tracks: " << emajor->getNumberOfTracks() << endl;
	cout << "Ticks per quarter note: " << emajor->getTicksPerQuarterNote() << endl;
	cout << "BPM: " << emajor->getBPM() << endl;

    Track * track1 = emajor->getTrack(0);
		
    for (int i = 0; i < track1->length; i++) {
        std::vector<MIDIEvent*> eventList = track1->MIDIEvents;
        MIDIEvent * event = eventList[i];
        cout << "delT: " << event->getDeltaT() << "; status: " <<
            event->getEventType() << "; channel: " << event->getChannel()
            << "; pitch: " << event->getParam1() << "; velocity: " <<
            event->getParam2() << endl;
    }
	return 0;
}

