// #include <LiquidCrystal.h>

#include <LiquidCrystal.h>
#include "SpotifyUtils.h"

// // Pin definitions
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
const int rs = 0, en = 1, d4 = 10, d5 = 11, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
long lastUpdate;
int timeBetweenUpdate = 1000;
int barsFilled = 0;

void setupUtils() {
  pinMode(PLAY_PAUSE_PIN, INPUT);
  pinMode(SKIP_PIN, INPUT);

  // Setup LCD
  lcd.begin(16,2);
  lcd.clear();
  lastUpdate = millis();

  //Reset lcd cursor to beginning of progress bar
  lcd.setCursor(0, 0);

  // draw empty progress bar
  displayProgressBar();
}

void displaySongName(String songName) {
  //truncate song name to 16 characters 
  songName = songName.substring(0, 16);

  // Display the song name on the second row
  lcd.setCursor(0, 1); // Set cursor to the second row
  lcd.print("                "); //clear line by printing spaces
  lcd.setCursor(0, 1); // reset to start of row
  lcd.print(songName); // print the song name
  lcd.setCursor(0, 0); //reset cursor to first row for progress bar
}

void displayProgressBar() {
  // Write empty progress bar 
  for(int i=0; i<16; i++) {
    lcd.write("-");
  }
}

void updateProgressBar(long mils) {
  //Add block to progress bar every timeBetweenUpdate
  if(mils - lastUpdate >= timeBetweenUpdate && barsFilled < 16) {
    lcd.write(byte(255));
    barsFilled++;
    lastUpdate = mils;
  }
}
