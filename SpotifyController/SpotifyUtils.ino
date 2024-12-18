#include <LiquidCrystal.h>
#include "SpotifyUtils.h"

const int PLAY_PAUSE_PIN = 8;
const int SKIP_PIN = 9;
int playPauseState = LOW;
int skipState = LOW;
int lastPlayPauseState = LOW;
int lastSkipState = LOW;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;  
const int rs = 0, en = 1, d4 = 10, d5 = 11, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
long timeBetweenUpdate = 1000;
int barsFilled = 0;

void setupUtils() {
  pinMode(PLAY_PAUSE_PIN, INPUT);
  pinMode(SKIP_PIN, INPUT);
  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(0, 0);
  displayProgressBar();
}

void displaySongName(String songName) {
  songName = songName.substring(0, 16);

  lcd.setCursor(0, 1); 
  lcd.print("                "); 
  lcd.setCursor(0, 1); 
  lcd.print(songName); 
  lcd.setCursor(0, 0); 
}

void displayProgressBar() {
  lcd.setCursor(0, 0);
  for(int i=0; i<16; i++) {
    lcd.write("-");
  }
  barsFilled = 0;
}

void updateProgressBar(long mils) {
  if((mils / timeBetweenUpdate > barsFilled) && (barsFilled < 16)) {
    lcd.write(byte(255));
    barsFilled++;
  }
}

void setTimeBetweenUpdate(long mils) {
  timeBetweenUpdate = mils;
}