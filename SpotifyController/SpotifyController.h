#ifndef SPOTIFY_CONTROLLER_H
#define SPOTIFY_CONTROLLER_H

#include <Arduino.h>
#include <ArduinoJson.h>

#define SPOTIFY_HOST "api.spotify.com"
#define SPOTIFY_ACCOUNTS_HOST "accounts.spotify.com"

#define SPOTIFY_PLAYER_ENDPOINT "/v1/me/player"
#define SPOTIFY_DEVICES_ENDPOINT "/v1/me/player/devices"

#define SPOTIFY_PLAY_ENDPOINT "/v1/me/player/play"
#define SPOTIFY_PAUSE_ENDPOINT "/v1/me/player/pause"
#define SPOTIFY_NEXT_TRACK_ENDPOINT "/v1/me/player/next"

#define SPOTIFY_TOKEN_ENDPOINT "/api/token"

#define SPOTIFY_ACCESS_TOKEN_LENGTH 309

char _bearerToken[SPOTIFY_ACCESS_TOKEN_LENGTH + 10];

char serialBuffer[1024];
int bytesRead = 0;
int writtenTo = 0;
//FSM States Enum
typedef enum {
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

#endif
