/*
 * Track.h
 * Ryan Geary
 */

#ifndef Track_h 
#define Track_h 

#include "NoteEvent.h"

class Track 
{
  public:
    Track() {

    }


  private:
    int currentNote;
    NoteEvent * noteEvents[];
    int length;


};

#endif
