#include <LiquidCrystal.h>


// Pin definitions
const int PLAY_PAUSE_PIN = 8;
const int SKIP_PIN = 9;

// Button states and debouncing variables
int playPauseState = LOW;
int skipState = LOW;
int lastPlayPauseState = LOW;
int lastSkipState = LOW;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;  // Adjust if needed

//LCD Variables
const int rs = 0, en = 1, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int lastUpdate;
int timeBetweenUpdate = 1000;
int barsFilled = 0;

// void setupUtils() {
//   pinMode(PLAY_PAUSE_PIN, INPUT);
//   pinMode(SKIP_PIN, INPUT);

//   // Setup LCD
//   lcd.begin(16,2);
//   lastUpdate = millis();
//   // Write empty progress bar 
//   for(int i=0; i<16; i++) {
//     lcd.write("-");
//   }
//   //Reset lcd cursor to beginning of progress bar
//   lcd.setCursor(0, 0);
// }
void setupUtils() {
  pinMode(PLAY_PAUSE_PIN, INPUT);
  pinMode(SKIP_PIN, INPUT);

  // Initialize the LCD
  lcd.begin(16, 2); // Set up the LCD with 16 columns and 2 rows
  lcd.clear();      // Clear the screen to ensure it's clean

  // Initialize the progress bar
  lcd.setCursor(0, 0);
  for (int i = 0; i < 16; i++) {
    lcd.write('-'); // Write dashes to indicate an empty progress bar
  }

  displaySongName();
}

//Read in button presses
void updateInputs() {
  // Read current states
  int playPauseReading = digitalRead(PLAY_PAUSE_PIN);
  int skipReading = digitalRead(SKIP_PIN);

  // Check for any button state change
  if (playPauseReading != lastPlayPauseState || skipReading != lastSkipState) {
    lastDebounceTime = millis();
  }

  // If enough time has passed, process the button presses
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Check play/pause button
    if (playPauseReading != playPauseState) {
      playPauseState = playPauseReading;
      if (playPauseState == HIGH) {
        lastButtonPressed = PLAY_PAUSE;
      }
    }
    
    // Check skip button
    if (skipReading != skipState) {
      skipState = skipReading;
      if (skipState == HIGH) {
        lastButtonPressed = SKIP;
      }
    }
  }

  // Save states for next iteration
  lastPlayPauseState = playPauseReading;
  lastSkipState = skipReading;
}


void displaySongName() {
  const int songLength = 16000; // Simulated song length in milliseconds (16 seconds for simplicity)
  const String songName = "Dummy Song";
  int progress = 0;

  // Clear the LCD to avoid leftover characters
  lcd.clear();

  // Display the song name on the second row
  lcd.setCursor(0, 1); // Set cursor to the second row
  lcd.print(songName); // Print the dummy song name

  unsigned long startTime = millis();
  while (progress < 16) {
    unsigned long currentTime = millis();
    if (currentTime - lastUpdate >= timeBetweenUpdate) {
      lcd.setCursor(progress, 0);  // Set the cursor to the progress bar position (first row)
      lcd.write(byte(255));       // Write a block (filled progress indicator)
      progress++;
      lastUpdate = currentTime;
    }

    // Exit the loop early if the simulated song time has elapsed
    if (currentTime - startTime >= songLength) {
      break;
    }
  }

  // Reset for the next song
  barsFilled = 0;
  lastUpdate = millis();
}


void displayProgressBar(long mils) {
  //Add block to progress bar every timeBetweenUpdate
  if(mils - lastUpdate >= timeBetweenUpdate && barsFilled < 16) {
    lcd.write(byte(255));
    barsFilled++;
    lastUpdate = mils;
  }
}