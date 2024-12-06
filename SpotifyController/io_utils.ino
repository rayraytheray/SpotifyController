
void writeToBuf() {
  int c = Serial.read();
  serialBuffer[bytesRead] = c;
  bytesRead++;
  writtenTo = 1;
}
// Only ever called if writtenTo is true
void readResponse() {
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