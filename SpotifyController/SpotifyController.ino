#include "SpotifyController.h"
#include "SpotifyUtils.h"
#include "io_utils.h"

void setup() {
  Serial.begin(115200); Serial.println();
  Serial.println("OK"); // let the python code know we are ready
  setupUtils();
}

void loop() {
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
  Serial.println(potValue);     // Send value to the computer
  delay(100); 
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
