bool sentPlay = false;
bool sentPause = false;
bool sentSkip = false;

// Input state structure
typedef struct {
    unsigned long songDuration;
    unsigned long songSyncTime;
    bool skipFlag;
    bool playFlag;
} state_vars;

bool testTransition(state startState,
                     state endState,
                     state_vars startStateVars,
                     state_vars endStateVars,
                     bool verbos);

char* s2str(state s) {
    switch(s) {
        case sWAIT_FOR_SONG:
            return "(1) WAIT_FOR_SONG";
        case sPAUSED:
            return "(2) PAUSED";
        case sPLAYING:
            return "(3) PLAYING";
        default:
            return "???";
    }
}

bool testTransition(state startState,
                   state endState,
                   state_vars startStateVars,
                   state_vars endStateVars,
                   bool verbos) {
    // Set up initial state
    songDuration = startStateVars.songDuration;
    songSyncTime = startStateVars.songSyncTime;
    skipFlag = startStateVars.skipFlag;
    playFlag = startStateVars.playFlag;
    
    // Update FSM
    state resultState = updateFSM(startState);
    
    // Check if test passed
    bool passedTest = (endState == resultState &&
                      songDuration == endStateVars.songDuration &&
                      songSyncTime == endStateVars.songSyncTime &&
                      skipFlag == endStateVars.skipFlag &&
                      playFlag == endStateVars.playFlag);
    
    if (!verbos) {
        return passedTest;
    } else if (passedTest) {
        char sToPrint[200];
        sprintf(sToPrint, "Test from %s to %s PASSED", s2str(startState), s2str(endState));
        Serial.println(sToPrint);
        return true;
    } else {
        char sToPrint[200];
        Serial.println(s2str(startState));
        sprintf(sToPrint, "Test from %s to %s FAILED", s2str(startState), s2str(endState));
        Serial.println(sToPrint);
        sprintf(sToPrint, "End state expected: %s | actual: %s", s2str(endState), s2str(resultState));
        Serial.println(sToPrint);
        Serial.println("State Variables:");
        Serial.print("skipFlag expected: "); Serial.print(endStateVars.skipFlag); 
        Serial.print(" actual: "); Serial.println(skipFlag);
        Serial.print("playFlag expected: "); Serial.print(endStateVars.playFlag); 
        Serial.print(" actual: "); Serial.println(playFlag);
        return false;
    }
}

// Test cases array definitions
const state testStatesIn[6] = {
    sWAIT_FOR_SONG,  // Test 1: Wait -> Wait when no song
    sPAUSED,         // Test 2: Paused -> Playing on play
    sPAUSED,         // Test 3: Paused -> Playing on skip
    sPLAYING,        // Test 4: Playing -> Paused on pause
    sPLAYING         // Test 5: Playing -> Playing on skip
};

const state testStatesOut[6] = {
    sWAIT_FOR_SONG,
    sPLAYING,
    sPLAYING,
    sPAUSED,
    sPLAYING
};

const state_vars testVarsIn[6] = {
    {5000, 0, false, false}, 
    {5000, 0, false, true},       
    {5000, 0, true, false},        
    {5000, 0, false, true},        
    {5000, 0, true, false}         
};

const state_vars testVarsOut[6] = {      
    {5000, 0, false, false}, 
    {5000, 0, false, false},       
    {5000, 0, false, false},       
    {5000, 0, false, false},        
    {5000, 0, false, false}        
};

bool testAllTests() {
    for (int i = 0; i < 5; i++) {
        Serial.print("Running test ");
        Serial.println(i);
        if (!testTransition(testStatesIn[i], testStatesOut[i], testVarsIn[i], testVarsOut[i], true)) {
            return false;
        }
        Serial.println();
    }
    Serial.println("All tests passed!");
    return true;
}

// void testCommunication() {
//   Serial.println("play");
//   sentPlay = true;
//   Serial.println("pause");
//   sentPause = true;
//   Serial.println("skip");
//   sentSkip = true;
// }

// void parseResponse(JsonDocument doc) {
  
// }
