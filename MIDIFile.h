/*
 * MIDIFile.h - library for using MIDI files with Arduino
 * Ryan Geary
 */

#ifndef MIDIFile_h
#define MIDIFile_h

// MIDI file format
enum MIDIFormat { 0, 1, 2 }
/*
 * 0 means the MIDI file consists of a header-chunk and a single track chunk
 * 1 means the MIDI file has 1 or more track chunks to be played simultaneously
 * 2 means the MIDI file has 1 or more independant tracks
 */

class MIDIFile 
{
  public:
    MIDIFile(File midi) {

    }


  private:
    Track * track[];
    int chunkLength;
    int ticksPerQuarterNote;
    MIDIFormat format;
    int numberOfTracks;
    int BPM;

}

#endif
