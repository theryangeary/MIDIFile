/*
 * Track.h
 * Ryan Geary
 */

#ifndef Track_h 
#define Track_h 

#include "MIDIEvent.h"
#include <vector>

class Track 
{
    int currentNote;
    std::vector<MIDIEvent *> MIDIEvents;
    int length;
    int throwaway;
  public:
    Track(int approxLength); 
    void addEvent(MIDIEvent * midiEvent); 
        int returnInt() {
            return 5;
        }

        int getLength() {
            return length;
        }

        std::vector<MIDIEvent *> getEventList() {
            return MIDIEvents;
        }

  private:

};

#endif
