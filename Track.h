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
    int currentNote;
    std::vector<MIDIEvent *> MIDIEvents;
    int throwaway;
    int length;
    
    void addEvent(MIDIEvent * midiEvent);
    Track(int approxLength);
};

Track::Track(int approxLength) {
    MIDIEvents.resize(approxLength);
    length = 0;
    throwaway = 1;
}

void Track::addEvent(MIDIEvent * midiEvent) {
  //std::cout << "test" << std::endl;
  if (MIDIEvents.capacity() < length) {
    MIDIEvents.resize(length+1);
  }
    MIDIEvents[length] = midiEvent;
    length++;
  //std::cout << length << std::endl;
}

#endif
