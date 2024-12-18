#include "SongTimer.h"

void testTimer() {
  //test start and getElapsedTime
  SongTimer songTimer;
  songTimer.start();
  delay(100);
  unsigned long time = songTimer.getElapsedTime();
  if(time >= 90 && time <= 110) {
    Serial.println("Timer test 1 passed");
  }
  else {
    Serial.println("Timer test 1 failed");
  }

  //test reset 
  songTimer.reset();
  delay(50);
  time = songTimer.getElapsedTime();
  if(time == 0) {
    Serial.println("Timer test 2 passed");
  }
  else {
    Serial.println("Timer test 2 failed");
  }

  //test start, stop, start
  songTimer.start();
  delay(50);
  songTimer.stop();
  delay(50);
  songTimer.start();
  delay(50);
  time = songTimer.getElapsedTime();
  if(time >= 90 && time <= 110) {
    Serial.println("Timer test 3 passed");
  }
  else {
    Serial.println("Timer test 3 failed");
  }
}