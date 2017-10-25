/*
 * NoteEvent.h
 * Ryan Geary
 */

#ifndef NoteEvent_h
#define NoteEvent_h

enum EventType {
    NOTE_ON,
    NOTE_OFF,
    POLYPHONIC_KEY_PRESSURE, 
    CONTROLLER_CHANGE, 
    PROGRAM_CHANGE, 
    CHANNEL_KEY_PRESSURE,
    PITCH_BEND
};

class NoteEvent
{
  public:
    NoteEvent(int deltaT, EventType eventType, char pitch, char velocity) {

    }


  private:
    int deltaT;
    EventType eventType;
    char pitch;
    char velocity;


};

#endif


