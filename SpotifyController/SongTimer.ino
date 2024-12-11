#include "SongTimer.h"

SongTimer::SongTimer() {
  reset();
}

void SongTimer::start() {
  if (!isRunning) {
    startTime = millis();
    isRunning = true;
  }
}

void SongTimer::stop() {
  if (isRunning) {
    pausedTime = millis();
    totalTime += pausedTime - startTime;
    isRunning = false;
  }
}

void SongTimer::reset() {
  startTime = 0;
  totalTime = 0;
  pausedTime = 0;
  isRunning = false;
}

// Get elapsed time in milliseconds
unsigned long SongTimer::getElapsedTime() {
  if (isRunning) {
    return totalTime + (millis() - startTime);
  }
  return totalTime;
}

bool SongTimer::running() {
  return isRunning;
}