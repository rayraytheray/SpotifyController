#include "SpotifyController.h"
#include "SpotifyUtils.h"
#include "io_utils.h"

String songName = "Waiting For Song";
unsigned long songDuration = 0;
unsigned long songSyncTime = 0;

const int threshold = 10;
int last_pot_value = 0;

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
  while(!Serial);
  #ifdef TEST
    Serial.println("STARTING TESTS");
    testAllTests();
    testLCD();
    testTimer();
    Serial.println("test");
  #else
    // Set up LCD and buttons
    setupUtils();
    displaySongName(songName);

    // Set up button pin interrupts
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
  #endif 
}

void loop() {
  #ifndef TEST
    // Pet watchdog
    WDT.refresh();
    
    // echo back in uppercase what we received
    if (Serial.available()) {
      writeToBuf();
    } else {
      if (writtenTo) {
        JsonDocument doc = readResponse();
        parseResponse(doc);
      }
    }

    static state CURRENT_STATE = sWAIT_FOR_SONG;
    CURRENT_STATE = updateFSM(CURRENT_STATE);
    delay(10);

    int pot_value = analogRead(A0);
    if (0 <= pot_value && pot_value <= 1023) {
      // Send updated volume only if the change exceeds the threshold
      if (abs(pot_value - last_pot_value) > threshold) {
        Serial.print("volume ");
        Serial.println(pot_value);
        last_pot_value = pot_value;
      }   
    }
  #endif 
}

state updateFSM(state curState) {
  state nextState = curState;
  switch(curState) {
  case sWAIT_FOR_SONG:
    if (songName != "Waiting For Song") {
      nextState = sPAUSED;
    }
    break;
  case sPAUSED:
    if (skipFlag) { //if skip was pressed
      skipFlag = false;
      Serial.println("skip");
      displayProgressBar();
      songTimer.reset();
      songTimer.start();
      nextState = sPLAYING;
    }
    else if (playFlag) { //if play was pressed
      Serial.println("play");
      playFlag = false;
      songTimer.start();
      nextState = sPLAYING;
    }
    break;
  case sPLAYING: {
    unsigned long songElapsed = songTimer.getElapsedTime();
    updateProgressBar(songElapsed);
    if (skipFlag) { //if skip was pressed
      skipFlag = false;
      Serial.println("skip");
      displayProgressBar();
      songTimer.reset();
      songTimer.start();
    }
    //if the song has reached the end, request new song info from API
    else if(songElapsed >= (songDuration + 1000)) { //1 second of delay to be safe
      Serial.println("getSongInfo");
      displayProgressBar(); //clear progress bar
      songTimer.reset();
    }
    else if (playFlag) { //if pause was pressed 
      Serial.println("pause");
      playFlag = false;
      nextState = sPAUSED;
      songTimer.stop();
    }
    break;
  }
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
    Serial.println("Doc is null in parseResponse");
    return;
  }
  #ifdef TEST 
  if(doc["PING"]) {
    Serial.print(doc["PING"].as<const char*>());
    Serial.println(": PONG");
  }
  #endif
  const char* message = doc["message"];
  if(message) {
    Serial.println(message);
  }
  const char* name = doc["name"];
  if(name) {
    songName = name;
    songDuration = doc["duration"].as<unsigned long>();
    songSyncTime = doc["progress"].as<unsigned long>();
    songTimer.start(songSyncTime); //sync local timer and API response
    setTimeBetweenUpdate(songDuration / 16);
    displaySongName(songName);
  }
}
