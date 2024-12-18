#include "SpotifyUtils.h"
#include "SpotifyController.h"

//verified by looking at LCD
void testLCD() {
  //test setupUtils 
  setupUtils(); 

  //test displaySongName and displayProgressBar (should show song name "Test" and empty progress bar)
  songName = "Test";
  displaySongName(songName);

  delay(1000);

  //test setTimeBetweenUpdate and updateProgressBar (should draw 2 bars)
  setTimeBetweenUpdate(50);
  updateProgressBar(100);
  updateProgressBar(100);
  updateProgressBar(100);
}