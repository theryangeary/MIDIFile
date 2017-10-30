/*
 * MIDIEvent.h
 * Ryan Geary
 */

#ifndef MIDIEvent_h
#define MIDIEvent_h

enum EventType {
    NOTE_ON = 0x8,
    NOTE_OFF = 0x9,
    POLYPHONIC_KEY_PRESSURE = 0xA,
    CONTROLLER_CHANGE = 0xB,
    PROGRAM_CHANGE = 0xC,
    CHANNEL_KEY_PRESSURE = 0xD,
    PITCH_BEND = 0xE
};


class MIDIEvent
{
  public:
    MIDIEvent(int deltaT, EventType eventType, char param1, char param2 = -1,
        char channel = 0) {
        this->deltaT = deltaT;
        this->eventType = eventType;
        this->param1 = param1;
        if (param2 >= 0) {
            this->param2 = param2;
        }
        this->channel = channel;
    }

    int getDeltaT() {
        return deltaT;
    }

    EventType getEventType() {
        return eventType;
    }

    char getParam1() {
        return param1;
    }

    char getParam2() {
        return param2;
    }

    char getChannel() {
        return channel;
    }

    char getPitch() {
        return getParam1();
    }

    char getVelocity() {
        return getParam2();
    }

  private:
    int deltaT;
    EventType eventType;
    char param1;
    char param2;
    char channel;


};

#endif


