#include "SpotifyController.h"
#include "SpotifyUtils.h"
#include "io_utils.h"
#include <FspTimer.h>

FspTimer watchdogTimer;
const int playPin = 2;  // play/pause button
const int skipPin = 3;  // skip button
volatile bool playFlag = false;
volatile bool skipFlag = false;

void setup() {
  // Set up serial
  Serial.begin(115200); 
  Serial.println();

  Serial.println("OK"); // let the python code know we are ready

  // Configure watchdog timer 
  uint32_t period_counts = 1000000; // 1 second in microseconds
  if (!watchdogTimer.begin(TIMER_MODE_PERIODIC, AGT_TIMER, 0, period_counts, 0, TIMER_SOURCE_DIV_8, wdtISR)) {
    Serial.println("Watchdog Timer configuration failed!");
    return;
  }
  // Start the timer
  watchdogTimer.start();

  // Set up LCD and buttons
  setupUtils();

  // set up button pin interrupt
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, FALLING);
}

void loop() {
  // Pet watchdog
  watchdogTimer.stop();
  watchdogTimer.start();

  if (interruptFlag) {
    Serial.println("Interrupt triggered!");
    interruptFlag = false;
  }

  // echo back in uppercase what we received
  if (Serial.available()) {
    writeToBuf();
  } else {
    if (writtenTo) {
      JsonDocument doc = readResponse();
    }
  }

  static state CURRENT_STATE = sWAIT_FOR_SONG;
  updateInputs();
  CURRENT_STATE = updateFSM(CURRENT_STATE, millis(), lastButtonPressed);
  delay(10);

    
  int potValue = analogRead(A0); // Read potentiometer value (0-1023)
  Serial.print("volume ");     // Send value to the computer
  Serial.println(potValue);
}

state updateFSM(state curState, long mils, int lastButton) {
  state nextState;
  switch(curState) {
  case sWAIT_FOR_SONG:
    displayProgressBar();
    break;
  case sPAUSED:
    displaySongName();
    break;
  case sPLAYING:
    displaySongName();
    updateProgressBar(mils);
    break;
  case sSKIPPING:
    break;
  }
  return nextState;
}

void handleInterrupt() {
  interruptFlag = true;
}

/* ISR when WDT triggers */
void wdtISR(timer_callback_args_t *arg) {
  Serial.println("System failure detected!");
  while(true);
}
