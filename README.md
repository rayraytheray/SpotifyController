# SpotifyController
Code for an Arduino based spotify controller

# How to Run
First set up Python environment using venv and pip install the necessary modules.
While in venv, first run the Python program using `python3 main.py`.
This will prompt you to enter the port number for the Arduino, which should be clear since we 
are only using one Arduino. 
Then, go into the Arduino IDE and upload the code onto the Arduino. **Shut down the Arduino IDE after this.**  
The Python code cannot detect the Arduino if not! 
After that, press the reset button on the Arduino to allow for operation. 
Logs should read Arduino ready, after which system is finished loading.

## TODO:
- Make all API requests in Python
- For Arduino communication, Serial.println() suffices. Make sure each message ends with a newline! 
- To have the Arduino request the computer do something, pass in some kind of unique message, ie want to play paused audio, Serial.println("play")