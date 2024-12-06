#include "SpotifyController.h"

void setup() {
  Serial.begin(115200); Serial.println();
  Serial.println("OK"); // let the python code know we are ready
}

void loop() {
  if (Serial.available()) {
    writeToBuf();
  } else {
    if (writtenTo) {
      JsonDocument doc = readResponse();
    }
  }
}
