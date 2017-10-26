#include "MIDIFile.h"
#include <iostream>
#include <fstream>

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
	return 0;
}

