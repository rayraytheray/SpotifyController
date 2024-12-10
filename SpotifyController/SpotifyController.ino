#include "SpotifyController.h"
#include "SpotifyUtils.h"
#include "io_utils.h"

String songName = "Example Song";
int songDuration = 0;

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
  setupUtils();

  // Set up button pin interrupt
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
    WDT.refresh();
  } else {
    Serial.println("Error initializing watchdog");
    while(1){}
  }

  //get initial song info
  Serial.println("getSongDuration");
}

void loop() {
  // Pet watchdog
  WDT.refresh();

  // echo back in uppercase what we received
  JsonDocument doc;
  if (Serial.available()) {
    writeToBuf();
  } else {
    if (writtenTo) {
      doc = readResponse();
    }
  }

  static state CURRENT_STATE = sWAIT_FOR_SONG;
  CURRENT_STATE = updateFSM(CURRENT_STATE, millis(), lastButtonPressed, doc);
  delay(10);

  int potValue = analogRead(A0); // Read potentiometer value (0-1023)
  // Serial.print("volume ");     // Send value to the computer
  // Serial.println(potValue);

}

state updateFSM(state curState, long mils, int lastButton, JsonDocument doc) {
  state nextState = curState;
  switch(curState) {
  case sWAIT_FOR_SONG:
    if(!doc.isNull()) {
      songName = doc["name"].as<String>();
      songDuration = doc["duration"].as<int>();
      nextState = sPAUSED;
    }
    break;
  case sPAUSED:
    displaySongName(songName);
    if (skipFlag) {
      skipFlag = false;
    }
    else if (playFlag) {
      Serial.println("play");
      playFlag = false;
      nextState = sPLAYING;
    }
    break;
  case sPLAYING:
    displaySongName(songName);
    updateProgressBar(mils);
    if (skipFlag) {
      skipFlag = false;
    }
    else if (playFlag) {
      Serial.println("pause");
      playFlag = false;
      nextState = sPAUSED;
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
      playFlag = !playFlag;
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
