#include "SpotifyController.h"
#include "SpotifyUtils.h"
#include "io_utils.h"

// Button setup
const unsigned long DEBOUNCE_TIME = 500;  // 200ms debounce
volatile unsigned long lastPlayPress = 0;
volatile unsigned long lastSkipPress = 0;
const int playPin = 2;  // play/pause button
const int skipPin = 3;  // skip button
volatile bool playFlag = false;
volatile bool skipFlag = false;


// Watchdog timer
const long wdtInterval = 2684;
unsigned long wdtMillis = 0;
void setup() {
  // Set up serial
  Serial.begin(115200); 
  Serial.println();

  Serial.println("OK"); // let the python code know we are ready

  // Set up LCD and buttons
  // Serial.println("Setting up utils");
  setupUtils();

  // Set up button pin interrupt
  // Serial.println("Setting up interrupts");
  pinMode(playPin, INPUT_PULLUP);
  pinMode(skipPin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(playPin), handlePlay, FALLING);
  attachInterrupt(digitalPinToInterrupt(skipPin), handleSkip, FALLING);

  // Start Watchdog Timer
  if(wdtInterval < 1) {
    Serial.println("Invalid watchdog interval");
    while(1){}
  }

  if(WDT.begin(wdtInterval)) {
    Serial.print("WDT interval: ");
    WDT.refresh();
    Serial.print(WDT.getTimeout());
    WDT.refresh();
    Serial.println(" ms");
    WDT.refresh();
  } else {
    Serial.println("Error initializing watchdog");
    while(1){}
  }
}

void loop() {
  // Pet watchdog
  WDT.refresh();

  // echo back in uppercase what we received
  JsonDocument doc = nullptr;
  if (Serial.available()) {
    writeToBuf();
  } else {
    if (writtenTo) {
      doc = readResponse();
    }
  }

  static state CURRENT_STATE = sPAUSED;
  updateInputs();
  CURRENT_STATE = updateFSM(CURRENT_STATE, millis(), lastButtonPressed);

  int potValue = analogRead(A0); // Read potentiometer value (0-1023)
  Serial.print("volume ");     // Send value to the computer
  Serial.println(potValue);
  // delay(100);
}

state updateFSM(state curState, long mils, int lastButton) {
  state nextState = curState;
  switch(curState) {
  case sWAIT_FOR_SONG:
    break;
  case sPAUSED:
    displaySongName();
    if (skipFlag) {
      skipFlag = false;
    }
    else if (playFlag) {
      playFlag = false;
      nextState = sPLAYING;
    }
    break;
  case sPLAYING:
    displaySongName();
    updateProgressBar(mils);
    if (playFlag) {
      playFlag = false;
      nextState = sPAUSED;
    }
    if (skipFlag) {
      skipFlag = false;
    }
    break;
  case sSKIPPING:
    break;
  default: 
    nextState = sWAIT_FOR_SONG;
  }
  // Serial.println(nextState);
  return nextState;
}

void handlePlay() {
  unsigned long currentTime = millis();
  if (currentTime - lastPlayPress > DEBOUNCE_TIME) {
      playFlag = true;
      lastPlayPress = currentTime;
  }
}

void handleSkip() {
  unsigned long currentTime = millis();
  if (currentTime - lastSkipPress > DEBOUNCE_TIME) {
      skipFlag = true;
      lastSkipPress = currentTime;
  }
}
