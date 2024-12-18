#ifndef SPOTIFY_CONTROLLER_TESTS_H
#define SPOTIFY_CONTROLLER_TESTS_H

#include "SpotifyController.h"

typedef struct {
    int lastButton;
    String currentSong;
    unsigned long songElapsed;
    bool skipButtonPressed;
    bool playButtonPressed;
} state_inputs;

typedef struct {
    String songName;
    unsigned long songDuration;
    unsigned long songSyncTime;
    bool skipFlag;
    bool playFlag;
} state_vars;

char* s2str(state s);
bool testTransition(state startState, 
                   state endState,
                   state_inputs testStateInputs, 
                   state_vars startStateVars,
                   state_vars endStateVars,
                   bool verbos);
bool testAllTests();

#endif // SPOTIFY_CONTROLLER_TESTS_H
