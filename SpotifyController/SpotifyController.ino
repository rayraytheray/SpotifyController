/*
  Find the full UNO R4 WiFi Network documentation here:
  https://docs.arduino.cc/tutorials/uno-r4-wifi/wifi-examples#wi-fi-web-client
 */

#include "WiFiS3.h"
#include "arduino_secrets.h" 
#include "SpotifyController.h"

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key index number (needed only for WEP)

int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char authServer[] = "accounts.spotify.com";
char server[] = "api.spotify.com";    // name address for Spotify's API (using DNS)

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;

/* -------------------------------------------------------------------------- */
void setup() {
/* -------------------------------------------------------------------------- */  
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
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
  
  printWifiStatus();

  // client.setCACert(SPOTIFY_SERVER_CERT);

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  // if (client.connect(authServer, 443)) {
  //   Serial.println("connected to server");
  //   // Make a HTTP request:
  //   client.println("POST /api/token HTTP/1.1");
  //   client.println("Host: accounts.spotify.com");
  //   client.println("User-Agent: Arduino/1.0");
  //   client.println("Content-Type: application/x-www-form-urlencoded");
  //   client.println("grant_type=client_credentials&client_id=9bbfd9ae49674d6e82a850e1fd4e2e12&client_secret=ae66c82bb5634a7c9fe282348b05dc2f");
  //   client.println("Connection: close");
  //   client.println();
  // }
}

/* just wrap the received data up to 80 columns in the serial print*/
/* -------------------------------------------------------------------------- */
void read_response() {
/* -------------------------------------------------------------------------- */  
  uint32_t received_data_num = 0;
  while (client.available()) {
    /* actual data reception */
    char c = client.read();
    /* print data to serial port */
    Serial.print(c);
    /* wrap data to 80 columns*/
    received_data_num++;
    if(received_data_num % 80 == 0) { 
      Serial.println();
    }
  }  
}

/* -------------------------------------------------------------------------- */
void loop() {
  client.flush();
/* -------------------------------------------------------------------------- */  
  if (client.connect(authServer, 80)) {
    char *body = "grant_type=client_credentials&client_id=9bbfd9ae49674d6e82a850e1fd4e2e12&client_secret=ae66c82bb5634a7c9fe282348b05dc2f";
    Serial.println("connected to server");
    // Make a HTTP request:
    client.println("POST /api/token HTTP/1.0");
    client.println("Host: accounts.spotify.com");
    client.println("Accept: application/json");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println("Cache-Control: no-cache");
    client.print("Content-Length: ");
    client.println(strlen(body));
    client.println();
    client.println("grant_type=client_credentials&client_id=9bbfd9ae49674d6e82a850e1fd4e2e12&client_secret=ae66c82bb5634a7c9fe282348b05dc2f");
    client.println("Connection: close");
    client.println();
    read_response();
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    // Serial.println();
    // Serial.println("disconnecting from server.");
    client.stop();
  }
}