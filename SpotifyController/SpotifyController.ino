#include "SpotifyController.h"
#include "SpotifyUtils.h"
#include "io_utils.h"

String songName = "Example Song";
long songDuration = 0;

// Button setup
const unsigned long DEBOUNCE_TIME = 500; // debounce time in ms
volatile unsigned long lastPlayPress = 0;
volatile unsigned long lastSkipPress = 0;
const int playPin = 2;  // play/pause button
const int skipPin = 3;  // skip button
volatile bool playFlag = false; //signal to fsm that button was pressed
volatile bool skipFlag = false; //signal to fsm that button was pressed

// Watchdog timer
const long wdtInterval = 2684; // wdt interval in ms

// Song timer 
SongTimer songTimer;

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
}

void loop() {
  // Pet watchdog
  WDT.refresh();
  
  // echo back in uppercase what we received
  if (Serial.available()) {
    writeToBuf();
  } else {
    if (writtenTo) {
      JsonDocument doc = readResponse();
      parseResponse(doc);
      Serial.println(songName);
    }
  }

  static state CURRENT_STATE = sWAIT_FOR_SONG;
  CURRENT_STATE = updateFSM(CURRENT_STATE, millis(), lastButtonPressed);
  delay(10);

  int potValue = analogRead(A0); // Read potentiometer value (0-1023)
  // Serial.print("volume ");     // Send value to the computer
  // Serial.println(potValue);
}

state updateFSM(state curState, long mils, int lastButton) {
  state nextState = curState;
  switch(curState) {
  case sWAIT_FOR_SONG:
    if (songName != "Example Song") {
      nextState = sPAUSED;
    }
    break;
  case sPAUSED:
    if (skipFlag) { //if skip was pressed
      skipFlag = false;
      Serial.println("skip");
      nextState = sPLAYING;
    }
    else if (playFlag) { //if pause was pressed
      Serial.println("play");
      playFlag = false;
      nextState = sPLAYING;
      songTimer.start();
    }
    break;
  case sPLAYING:
    updateProgressBar(mils);
    //if the song has reached the end, request new song info from API
    if(songTimer.getElapsedTime() >= (songDuration + 1000)) { //1 second of delay to be safe
      songTimer.reset();
      Serial.println("getSongDuration");
    }
    else if (skipFlag) { //if skip was pressed
      skipFlag = false;
      Serial.println("skip");
    }
    else if (playFlag) { //if pause was pressed 
      Serial.println("pause");
      playFlag = false;
      nextState = sPAUSED;
      songTimer.stop();
    }
    break;
  default: 
    nextState = sWAIT_FOR_SONG;
    break;
  }
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

void parseResponse(JsonDocument doc) {
  if(doc.isNull()) {
    return;
  }
  const char* message = doc["message"];
  if(message) {
    Serial.println(message);
  }
  const char* name = doc["name"];
  if(name) {
    songName = name;
    songDuration = doc["duration"].as<long>();
    displaySongName(songName);
  }
}
