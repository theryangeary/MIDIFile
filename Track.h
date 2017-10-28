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
  public:
    Track(int approxLength) {
        MIDIEvents.resize(approxLength);
        length = 0;
    };

    addEvent(MIDIEvent * midiEvent) {
        MIDIEvents[length] = midiEvent;
        length++;
    }


  private:
    int currentNote;
    std::vector<MIDIEvent *> MIDIEvents;
    int length;


};

#endif
