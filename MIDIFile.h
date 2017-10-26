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

#define HEADER_CHUNK_LENGTH_BYTE_LENGTH 4 
#define HEADER_CHUNK_DATA_BYTE_LENGTH 2

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

    int charArrayToInt(char* bytes, int numBytes) {
	    int result = 0;
	for (int i = 0; i < numBytes; i++) {
	       result = (result << 8) | bytes[i];
	}	       
	return result;
	}
  public:
    MIDIFile(ifstream * midiFile) {
	// initialize variables
	BPM = 120;

        // find the beginning of the MIDI data
        //midiFile.find('Mthd', 4); 
        // store MIDI header data
	char tmp[4];
        midiFile->read(tmp, 4); // read the header

	midiFile->read(tmp, 
			HEADER_CHUNK_LENGTH_BYTE_LENGTH); // read the chunk length
	headerChunkLength = charArrayToInt(tmp, HEADER_CHUNK_LENGTH_BYTE_LENGTH);

        midiFile->read(tmp, HEADER_CHUNK_DATA_BYTE_LENGTH); // read the format
	formatNumber = charArrayToInt(tmp, HEADER_CHUNK_DATA_BYTE_LENGTH);
	format = static_cast<MIDIFormat>(formatNumber); // set MIDIFormat enum

        midiFile->read(tmp, HEADER_CHUNK_DATA_BYTE_LENGTH); // read # of tracks
	numberOfTracks = charArrayToInt(tmp, HEADER_CHUNK_DATA_BYTE_LENGTH);

        midiFile->read(tmp, HEADER_CHUNK_DATA_BYTE_LENGTH); // read ticksPerQN
	ticksPerQuarterNote = charArrayToInt(tmp, HEADER_CHUNK_DATA_BYTE_LENGTH);

        // check header chunk size compared to 6, the intended Mthd size
        int runningLength = 0;
 //       int runningLength += sizeof(numberOfTracks);
        runningLength += sizeof headerChunkLength;
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
