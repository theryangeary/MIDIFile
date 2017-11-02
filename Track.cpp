#include "Track.h"

Track::Track(int approxLength) {
    MIDIEvents.resize(approxLength);
    length = 0;
    throwaway = 1;
}




void Track::addEvent(MIDIEvent * midiEvent) {
    MIDIEvents[length] = midiEvent;
    length++;
}
