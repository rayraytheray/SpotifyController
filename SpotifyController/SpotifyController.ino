/*
  Find the full UNO R4 WiFi Network documentation here:
  https://docs.arduino.cc/tutorials/uno-r4-wifi/wifi-examples#wi-fi-web-client
 */

void setup() {
  Serial.begin(115200); Serial.println();
  Serial.println("OK"); // let the python code know we are ready
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
}