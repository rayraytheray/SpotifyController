#ifndef SONG_TIMER_H
#define SONG_TIMER_H

#include "Arduino.h"

/* Class that acts as a stopwatch, keeping track of elapsed time in song */
class SongTimer {
  private:
    unsigned long startTime;     // When the stopwatch was last started
    unsigned long totalTime;     // Total accumulated time
    unsigned long pausedTime;    // total time when stopwatch was paused
    bool isRunning;             // Current running state
    
  public:
    SongTimer();
    void start();
    void stop();
    void reset();
    unsigned long getElapsedTime();
    bool running();
};

#endif