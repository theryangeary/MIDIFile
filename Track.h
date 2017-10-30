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

    void addEvent(MIDIEvent * midiEvent) {
        MIDIEvents[length] = midiEvent;
        length++;
    }

    int getLength() {
        std::cout << this->length << std::endl;
        return this->length;
    }

    std::vector<MIDIEvent *> getEventList() {
        return MIDIEvents;
    };

  private:
    int currentNote;
    std::vector<MIDIEvent *> MIDIEvents;
    int length;


};

#endif
