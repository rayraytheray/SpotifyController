#include <ArduinoUnit.h>
#include "SongTimer.h"

// Test initial state
test(initial_state) {
    SongTimer timer;
    assertEqual(timer.getElapsedTime(), 0UL);
    assertEqual(timer.running(), false);
}

// Test start function
test(start_function) {
    SongTimer timer;
    timer.start();
    assertEqual(timer.running(), true);
    delay(100);  // Wait 100ms
    assertTrue(timer.getElapsedTime() >= 100);
}

// Test stop function
test(stop_function) {
    SongTimer timer;
    timer.start();
    delay(100);
    timer.stop();
    unsigned long stoppedTime = timer.getElapsedTime();
    delay(100);
    assertEqual(timer.getElapsedTime(), stoppedTime);
}

// Test reset function
test(reset_function) {
    SongTimer timer;
    timer.start();
    delay(100);
    timer.reset();
    assertEqual(timer.getElapsedTime(), 0UL);
    assertEqual(timer.running(), false);
}

void setup() {
    Serial.begin(9600);
    while(!Serial); // Wait for Serial to be ready
}

void loop() {
    Test::run();
}