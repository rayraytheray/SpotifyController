#ifndef SPOTIFY_CONTROLLER_H
#define SPOTIFY_CONTROLLER_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WDT.h>
#include "SongTimer.h"

#define TEST

char serialBuffer[1024];
int bytesRead = 0;
int writtenTo = 0;
//FSM States Enum
typedef enum {
  sWAIT_FOR_SONG = 1,
  sPAUSED = 2,
  sPLAYING = 3,
} state;

//Buttons Enum
typedef enum {
  PLAY_PAUSE = 0,
  SKIP = 1,
} input;

//Variable to keep track of inputs
input lastButtonPressed;

#endif
