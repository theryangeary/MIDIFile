/*
 * MIDIFile.h - library for using MIDI files with Arduino
 * Ryan Geary
 */

#ifndef MIDIFile_h
#define MIDIFile_h

#include <iostream>
#include <fstream>
#include "Track.h"
#include "MIDIEvent.h"
#include <string.h>

#define HEADER_CHUNK_LENGTH_BYTE_LENGTH 4 
#define HEADER_CHUNK_DATA_BYTE_LENGTH 2
#define TRACK_CHUNK_LENGTH_BYTE_LENGTH 4
#define TRACK_CHUNK_DATA_BYTE_LENGTH 2
#define TRACK_CHUNK_HEADER_BYTE_LENGTH 4
#define NIBBLE_LENGTH 4
#define BYTE_LENGTH 8


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

//enum EventType {
//    NOTE_ON = 0x8,
//    NOTE_OFF = 0x9,
//    POLYPHONIC_KEY_PRESSURE = 0xA,
//    CONTROLLER_CHANGE = 0xB,
//    PROGRAM_CHANGE = 0xC,
//    CHANNEL_KEY_PRESSURE = 0xD,
//    PITCH_BEND = 0xE
//};


class MIDIFile 
{

  private:
    Track ** tracks;
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
			cout << result << endl;
    	return result;
	}

  public:
    MIDIFile(ifstream * midiFile) {
        // initialize variables
	    BPM = 120;

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
 	    runningLength += sizeof numberOfTracks;
        runningLength += sizeof headerChunkLength;
	    runningLength += sizeof ticksPerQuarterNote;

	    //if (runningLength == headerChunkLength) {
		    // done reading header chunk
            int trackNumber = 0;
            while (midiFile->good()) {
		        createTrack(midiFile, trackNumber); // make tracks		
                trackNumber++;
            }
	    //}
	    //else {
	        // there is a discrepancy between actual & expected header length	
	    //}
    };

    void createTrack(ifstream * midiFile, int trackNumber) {
        char tmp[4];
        midiFile->read(tmp, TRACK_CHUNK_HEADER_BYTE_LENGTH);;
        if (tmp[0] == 'M' && tmp[1] == 'T' && tmp[2] == 'r' && tmp[3] == 'k') {
				cout << "found a track" << endl;
            // continue making track
            midiFile->read(tmp, TRACK_CHUNK_LENGTH_BYTE_LENGTH);
            int trackLength = charArrayToInt(tmp, TRACK_CHUNK_LENGTH_BYTE_LENGTH);
            int runningLength = 0;
            // track size may need more fine tuned adjustment to be always big
            // enough but without wasting space
            Track * track = new Track(trackLength / 4);
// 						cout << "test1" << endl;
 						cout << runningLength << endl;
 						cout << trackLength << endl;
// 						cout << (runningLength <= trackLength) << endl;

            while (runningLength < trackLength) {
                int deltaT = 0;
                char currentTimeByte;
                do {
                    currentTimeByte = midiFile->get(); 
										runningLength++;
                    // concatenate the seven less significant bits of the time
                    // byte, because the MSB is only to show whether current
                    // byte is the last part of deltaT
                    (deltaT << 7) | (currentTimeByte << 1 >> 1);
                } while (currentTimeByte > 0x80);
                // status and channel are in the same byte
                char statusAndChannel = midiFile->get();
                EventType eventType = static_cast<EventType>(
                    (statusAndChannel >> NIBBLE_LENGTH));
                char channel = statusAndChannel << NIBBLE_LENGTH >> NIBBLE_LENGTH;
                char param1 = midiFile->get();
								runningLength++;
								cout << "Adding event: deltaT: " << deltaT << " param1: " << param1 << endl;
                if (statusAndChannel >> NIBBLE_LENGTH <= 0xC) {
                    char param2 = midiFile->get();
                    track->addEvent(new MIDIEvent(deltaT, eventType, param1,
                        param2, channel));
                }
                else {
                    track->addEvent(new MIDIEvent(deltaT, eventType, param1,
                        channel = channel));
                }
            }
            tracks[trackNumber] = track;
						cout << track << endl;
        }
        else {
				cout << "if False" <<endl;
            // handle the fact that this is not a track
            //cout << "This is not a track!" << endl;
        }
    }
    
    Track * getTrack(int trackNumber) {
        return tracks[trackNumber];
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
