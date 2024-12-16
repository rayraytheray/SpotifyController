#include "io_utils.h"

void writeToBuf() {
  while(Serial.available() && bytesRead < sizeof(serialBuffer) - 1) {
    int c = Serial.read();
    serialBuffer[bytesRead] = c;
    bytesRead++;
    
    // If we've received a newline, mark the message as complete
    if (c == '\n' || c == '\r') {
      serialBuffer[bytesRead] = 0;  // Null terminate
      writtenTo = 1;
      return;
    }
  }
}

// Only ever called if writtenTo is true
JsonDocument readResponse() {
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, serialBuffer);
  if (error) {
    Serial.print("Deserialization failed: ");
    Serial.println(error.c_str());
  }
  memset(serialBuffer, 0, sizeof serialBuffer);
  bytesRead = 0;
  writtenTo = 0;
  return doc;
}
