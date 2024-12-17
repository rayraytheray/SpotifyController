# SpotifyController
Code for an Arduino based spotify controller

# How to Run
First set up Python environment using venv and pip install the necessary modules.
Then, acquire the access token by putting in the following url into your browser:
https://accounts.spotify.com/authorize?response_type=token&client_id=ce52c9f4c06d4e24857ce7d900979937&scope=user-modify-playback-state%20user-read-currently-playing&redirect_uri=https://google.com&state=123
This should redirect to a Spotify page that asks you to login with your account. After signing in, it will redirect to google.com. Look in the URL, appended to google.com will be the necessary tokens in the format "www.google.com#access_token={token}&token_type={type}&expires_in={}". Take the token field and copy paste it as a string in main.py.
While in venv, first run the Python program using `python3 main.py`.
This will prompt you to enter the port number for the Arduino, which should be clear since we 
are only using one Arduino. 
Then, go into the Arduino IDE and upload the code onto the Arduino. **Shut down the Arduino IDE after this.**  
The Python code cannot detect the Arduino if not! 
After that, press the reset button on the Arduino to allow for operation. 
Logs should read Arduino ready, after which system is finished loading.
