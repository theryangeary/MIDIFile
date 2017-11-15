/*
 * MIDIFile.h - library for using MIDI files with Arduino
 * Ryan Geary
 */

#ifndef MIDIFile_h
#define MIDIFile_h

#include "../SdFat/src/FatLib/iostream.h"
#include "../SdFat/src/FatLib/fstream.h"
#include "../SdFat/src/FatLib/ios.h"
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


//using namespace::std;

// MIDI file format
enum MIDIFormat { 
    ZERO=0, 
    ONE=1, 
    TWO=2 
};

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
    	return result;
	}
	  int getVarLengthInt(fstream * midiFile, int * runningLength) {
		  int result = 0;
			int currentByte;
			do {
					currentByte = midiFile->get(); 
					*runningLength++;
					// concatenate the seven less significant bits of the time
					// byte, because the MSB is only to show whether current
					// byte is the last part of deltaT
					result = (result << 7) | (currentByte << 1 >> 1);
			} while (currentByte > 0x80);
			return result;
		}
		
		int skipForwardNBytes(fstream * midiFile, int * runningLength, int n) {
			for (int i = 0; i < n; i++) {
				midiFile->get();
				runningLength++;
			}
		}

  public:
    MIDIFile(fstream * midiFile) {
        // initialize variables
	    BPM = 120;
	    tracks = new Track*[1];
	    char tmp[4];

        // store MIDI header data
        midiFile->get(tmp, 4); // read the header
	    midiFile->get(tmp, 
			HEADER_CHUNK_LENGTH_BYTE_LENGTH); // get the chunk length
	    headerChunkLength = charArrayToInt(tmp, HEADER_CHUNK_LENGTH_BYTE_LENGTH);

        midiFile->get(tmp, HEADER_CHUNK_DATA_BYTE_LENGTH); // read the format
	    formatNumber = charArrayToInt(tmp, HEADER_CHUNK_DATA_BYTE_LENGTH);
	    format = static_cast<MIDIFormat>(formatNumber); // set MIDIFormat enum

        midiFile->get(tmp, HEADER_CHUNK_DATA_BYTE_LENGTH); // read # of tracks
	    numberOfTracks = charArrayToInt(tmp, HEADER_CHUNK_DATA_BYTE_LENGTH);

        midiFile->get(tmp, HEADER_CHUNK_DATA_BYTE_LENGTH); // read ticksPerQN
	    ticksPerQuarterNote = charArrayToInt(tmp, HEADER_CHUNK_DATA_BYTE_LENGTH);

        // TODO forthcoming check to make sure the header chunk is done with
        // check header chunk size compared to 6, the intended Mthd size
 	    //runningLength += sizeof numberOfTracks;
        //runningLength += sizeof headerChunkLength;
	    //runningLength += sizeof ticksPerQuarterNote;
			
	    //if (runningLength == headerChunkLength) {
		    // done geting header chunk
            int trackNumber = 0;
            while (midiFile->good()) {
		        createTrack(midiFile, trackNumber); // make tracks		
                trackNumber++;
            }
	    //}
	    //else {
	        // there is a discrepancy between actual & expected header length	
	    //}
	    // END TODO
    };

    void createTrack(fstream * midiFile, int trackNumber) {
        char tmp[4];
        midiFile->get(tmp, TRACK_CHUNK_HEADER_BYTE_LENGTH);
        if (tmp[0] == 'M' && tmp[1] == 'T' && tmp[2] == 'r' && tmp[3] == 'k') {
            // continue making track
            midiFile->get(tmp, TRACK_CHUNK_LENGTH_BYTE_LENGTH);
            int trackLength = charArrayToInt(tmp, TRACK_CHUNK_LENGTH_BYTE_LENGTH);
            int runningLength = 0;
            // track size may need more fine tuned adjustment to be always big
            // enough but without wasting space
            Track * track = new Track(trackLength / 4);
            while (runningLength < trackLength) {
                int deltaT = getVarLengthInt(midiFile, &runningLength);
                // status and channel are in the same byte
                int eventStartByte = midiFile->get();
                // TODO handle sysex events instead of ignoring them
                if (eventStartByte > 0xF0 && eventStartByte < 0xF7) { // sysex event
                    int length = getVarLengthInt(midiFile, &runningLength);
                            skipForwardNBytes(midiFile, &runningLength, length);
                }
                else if (eventStartByte == 0xFF) { // meta event
                    int secondByte = midiFile->get();
                    runningLength++;
                    // TODO handle meta events instead of ignoring them
                    if ((secondByte > 0 && secondByte < 8) || secondByte == 0x7F) {
                        int length = getVarLengthInt(midiFile, &runningLength);
                        for (int i = 0; i < length; i++) {
                            midiFile->get();
                        }
                    } else if (secondByte == 0x2F) {
                        skipForwardNBytes(midiFile, &runningLength, 1);
                        break;
                    } else if (secondByte == 0x20) {
                        skipForwardNBytes(midiFile, &runningLength, 2);
                    } else if (secondByte == 0 || secondByte == 0x59) {
                        skipForwardNBytes(midiFile, &runningLength, 3);
                    } else if (secondByte == 0x51) {
                        skipForwardNBytes(midiFile, &runningLength, 4);
                    } else if (secondByte == 0x58) {
                        skipForwardNBytes(midiFile, &runningLength, 5);
                    } else if (secondByte == 0x54) {
                        skipForwardNBytes(midiFile, &runningLength, 6);
                    }
                }
                else if (eventStartByte < 0xF0) { // midi event
                    int statusAndChannel = eventStartByte;
                    
                    EventType eventType = static_cast<EventType>(
                            (statusAndChannel >> NIBBLE_LENGTH));
                    int channel = statusAndChannel && 0x000000FF;
                    int param1 = midiFile->get();
                    runningLength++;
                    if (statusAndChannel >> NIBBLE_LENGTH <= 0xC) {
                            int param2 = midiFile->get();
                            track->addEvent(new MIDIEvent(deltaT, eventType, param1,
                                    param2, channel));
                    }
                    else {
                            track->addEvent(new MIDIEvent(deltaT, eventType, param1,
                                    channel = channel));
                    }
                }

            }
            tracks[trackNumber] = track;
        }
        else {
            // handle the fact that this is not a track
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
