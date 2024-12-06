#include <ArduinoJson.h>

#define SPOTIFY_ACCESS_TOKEN_LENGTH 309

char _bearerToken[SPOTIFY_ACCESS_TOKEN_LENGTH + 10];

char serialBuffer[1024];
int bytesRead = 0;
int writtenTo = 0;

// FSM States Enum
typedef enum {
  sWAIT_FOR_WIFI = 0,
  sWAIT_FOR_SONG = 1,
  sPAUSED = 2,
  sPLAYING = 3,
  sSKIPPING = 4,
} state;

//Buttons Enum
typedef enum {
  PLAY_PAUSE = 0,
  SKIP = 1,
} input;

//Variable to keep track of inputs
input lastButtonPressed;
