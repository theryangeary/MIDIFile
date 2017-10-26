/*
 * MIDIFile.h - library for using MIDI files with Arduino
 * Ryan Geary
 */

#ifndef MIDIFile_h
#define MIDIFile_h

#include <iostream>
#include <fstream>
#include "Track.h"
#include "NoteEvent.h"

using namespace::std;

// MIDI file format
enum MIDIFormat { 
    ZERO=0, 
    ONE=1, 
    TWO=2 
};
/*
 * 0 means the MIDI file consists of a header-chunk and a single track chunk
 * 1 means the MIDI file has 1 or more track chunks to be played simultaneously
 * 2 means the MIDI file has 1 or more independant tracks
 */

class MIDIFile 
{

  private:
    Track * track[];
    int headerChunkLength;
    int ticksPerQuarterNote;
    int formatNumber;
    MIDIFormat format;
    int numberOfTracks;
    int BPM;
  public:
    MIDIFile(ifstream * midiFile) {
	// initialize variables
	BPM = 120;

        // find the beginning of the MIDI data
        //midiFile.find('Mthd', 4); 
cout << "file test 1"<<endl;
        // store MIDI header data
	char tmp[1];
        midiFile->read(tmp, 1);
	cout << "file test 2" << endl;
	headerChunkLength = (int) tmp[0];
	cout << "file test 2.5" <<endl;
        midiFile->read(tmp, 1);
	formatNumber = (int) tmp[0];
	format = static_cast<MIDIFormat>(formatNumber);
	cout << "file test 3" <<endl;
        midiFile->read(tmp, 1); 
	numberOfTracks = (int) tmp[0];
        midiFile->read(tmp, 1);        
	ticksPerQuarterNote = (int) tmp[0];

        // check header chunk size compared to 6, the intended Mthd size
        int runningLength = 0;
 //       int runningLength += sizeof(numberOfTracks);
        runningLength += sizeof headerChunkLength;
	cout << "file test 4" <<endl;
  //      int runningLength += sizeof(ticksPerQuarterNote);
    };
    
    Track * getTrack(int trackNumber) {
        return track[trackNumber];
    }

    int getHeaderChunkLength() {
        return headerChunkLength;
    }

    int getTicksPerQuarterNote() {
        return ticksPerQuarterNote;
    }

    MIDIFormat getFormat() {
        return format;
    }

    int getNumberOfTracks() {
        return numberOfTracks;
    }

    int getBPM() {
        return BPM;
    }

};

#endif
