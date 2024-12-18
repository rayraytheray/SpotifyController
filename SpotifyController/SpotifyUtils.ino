#include <LiquidCrystal.h>
#include "SpotifyUtils.h"

// Button states and debouncing variables
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

/*
* Sets up the LCD and displays empty progress bar
*/
void setupUtils() {
  // Setup LCD
  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(0, 0);
  displayProgressBar();
}

/*
* Displays songName on the second row of the LCD, 
* replacing what was there before. 
*/
void displaySongName(String songName) {
  songName = songName.substring(0, 16);

  lcd.setCursor(0, 1); 
  lcd.print("                "); 
  lcd.setCursor(0, 1); 
  lcd.print(songName); 
  lcd.setCursor(0, 0); 
}

/*
* Displays an empty progress bar on the first row of the LCD
*/
void displayProgressBar() {
  lcd.setCursor(0, 0);
  for(int i=0; i<16; i++) {
    lcd.write("-");
  }
  barsFilled = 0;
}

/*
* Draws the appropriate number of bars based on 
* progress through the song
*/
void updateProgressBar(long mils) {
  if((mils / timeBetweenUpdate > barsFilled) && (barsFilled < 16)) {
    lcd.write(byte(255));
    barsFilled++;
  }
}

/*
* Set how long the song needs to progress before 
* each bar is drawn 
*/
void setTimeBetweenUpdate(long mils) {
  timeBetweenUpdate = mils;
}