#include "SpotifyController.h"
#include "SpotifyUtils.h"
#include "io_utils.h"

#define TIMER_FREQ_HZ  (64000000)  // Base clock frequency for GPT
#define WDT_TIMEOUT_MS 1000        // Desired timeout in milliseconds

FspTimer watchdogTimer;
const int playPin = 10;  // play/pause button
const int skipPin = 11;  // skip button
volatile bool playFlag = false;
volatile bool skipFlag = false;

void setup() {
  // Set up serial
  Serial.begin(115200); 
  Serial.println();

  Serial.println("OK"); // let the python code know we are ready

  // Set up LCD and buttons
  Serial.println("Setting up utils");
  setupUtils();

  // set up button pin interrupt
  Serial.println("Setting up interrupts");
  pinMode(playPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(playPin), handlePlay, FALLING);

  // Configure watchdog timer 
  uint32_t base_freq = TIMER_FREQ_HZ >> 3;  // Divide by 8 because of TIMER_SOURCE_DIV_8
  uint32_t period_counts = (base_freq / 1000) * WDT_TIMEOUT_MS;
  uint8_t timer_type = 0;  // Variable to hold the timer type

  int8_t channel = watchdogTimer.get_available_timer(timer_type);
  if (!watchdogTimer.begin(TIMER_MODE_PERIODIC, timer_type, channel, period_counts, 0, TIMER_SOURCE_DIV_8, wdtISR, nullptr)) {
    Serial.println("Watchdog Timer configuration failed!");
    return;
  }
  // Start the timer
  watchdogTimer.setup_overflow_irq();
  if(!watchdogTimer.open()) {
    Serial.println("Failed to open watchdog timer");
  }
  if(!watchdogTimer.start()) {
    Serial.println("Failed to start watchdog timer");
  }
}

void loop() {
  // Pet watchdog
  watchdogTimer.reset();

  if (playFlag) {
    Serial.println("Interrupt triggered!");
    playFlag = false;
  }

  // echo back in uppercase what we received
  if (Serial.available()) {
    writeToBuf();
  } else {
    if (writtenTo) {
      JsonDocument doc = readResponse();
    }
  }

  static state CURRENT_STATE = sWAIT_FOR_SONG;
  updateInputs();
  CURRENT_STATE = updateFSM(CURRENT_STATE, millis(), lastButtonPressed);

  int potValue = analogRead(A0); // Read potentiometer value (0-1023)
  Serial.print("volume ");     // Send value to the computer
  Serial.println(potValue);

}

state updateFSM(state curState, long mils, int lastButton) {
  state nextState;
  switch(curState) {
  case sWAIT_FOR_SONG:
    break;
  case sPAUSED:
    displaySongName();
    break;
  case sPLAYING:
    displaySongName();
    updateProgressBar(mils);
    watchdogTimer.reset();
    break;
  case sSKIPPING:
    break;
  }
  return nextState;
}

void handlePlay() {
  playFlag = true;
}

/* ISR when WDT triggers */
void wdtISR(timer_callback_args_t *arg) {
  Serial.println("Error: System failure");
}
