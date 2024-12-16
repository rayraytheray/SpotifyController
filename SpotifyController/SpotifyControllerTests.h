#ifndef SPOTIFY_CONTROLLER_TESTS_H
#define SPOTIFY_CONTROLLER_TESTS_H

#include "SpotifyController.h"

// Input state structure
typedef struct {
    int lastButton;
    String currentSong;
    unsigned long songElapsed;
    bool skipButtonPressed;
    bool playButtonPressed;
} state_inputs;

// Variables state structure
typedef struct {
    String songName;
    unsigned long songDuration;
    unsigned long songSyncTime;
    bool skipFlag;
    bool playFlag;
} state_vars;

// Function declarations
char* s2str(state s);
bool testTransition(state startState, 
                   state endState,
                   state_inputs testStateInputs, 
                   state_vars startStateVars,
                   state_vars endStateVars,
                   bool verbos);
bool testAllTests();
// state updateFSM(state currentState, int button);

#endif // SPOTIFY_CONTROLLER_TESTS_H