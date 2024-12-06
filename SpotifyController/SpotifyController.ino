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
/* -------------------------------------------------------------------------- */  
  static state CURRENT_STATE = sWAIT_FOR_WIFI;
  updateInputs();
  CURRENT_STATE = updateFSM(CURRENT_STATE, millis(), lastButtonPressed);
  delay(10);
  // if (playPauseState == HIGH) {
  //   displaySongName();
  // }
}

state updateFSM(state curState, long mils, int lastButton) {
  state nextState;
  switch(curState) {
  case sWAIT_FOR_WIFI: 
    if(wifiConnected) {
      nextState = sWAIT_FOR_SONG;
    }
    displayWaitingForWifi();
    connectWifi();
    break;
  case sWAIT_FOR_SONG:
    displayProgressBar();
    break;
  case sPAUSED:
    displaySongName();
    break;
  case sPLAYING:
    displaySongName();
    updateProgressBar(millis());
    break;
  case sSKIPPING:
    break;
  }
  return nextState;
}

void connectWifi() {
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }
  
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }
  
  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid);
     
    // wait 10 seconds for connection:
    delay(10000);
  }
  
  wifiConnected = true;
  printWifiStatus();
}

/* -------------------------------------------------------------------------- */
void printWifiStatus() {
/* -------------------------------------------------------------------------- */  
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
