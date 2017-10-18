/*
 * Track.h
 * Ryan Geary
 */

#ifndef Track_h 
#define Track_h 

class Track 
{
  public:
    Track() {
    }


  private:
    int currentNote = 0;
    NoteEvent * noteEvents[];
    int length;


}

}

#endif
