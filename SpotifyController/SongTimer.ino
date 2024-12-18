#include "SongTimer.h"

/*
* Stopwatch style class to keep track of the progress of a song
*/
SongTimer::SongTimer() {
  reset();
}

/*
* Starts the stopwatch
*/
void SongTimer::start(unsigned long syncTime) {
  if (!isRunning) {
    startTime = millis();
    isRunning = true;
    if(syncTime != 0) {
      totalTime = syncTime;
    }
  }
}

/*
* Stops the stopwatch
*/
void SongTimer::stop() {
  if (isRunning) {
    pausedTime = millis();
    totalTime += pausedTime - startTime;
    isRunning = false;
  }
}

/*
* Resets the stopwatch back to 0
*/
void SongTimer::reset() {
  startTime = 0;
  totalTime = 0;
  pausedTime = 0;
  isRunning = false;
}


/*
* Gets the stopwatch time in ms
*/
unsigned long SongTimer::getElapsedTime() {
  if (isRunning) {
    return totalTime + (millis() - startTime);
  }
  return totalTime;
}