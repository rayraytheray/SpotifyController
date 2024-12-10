#
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in
#  all copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
#  THE SOFTWARE.
#  ===============================================

#define SPOTIFY_HOST "api.spotify.com"
#define SPOTIFY_ACCOUNTS_HOST "accounts.spotify.com"

#define SPOTIFY_PLAYER_ENDPOINT "/v1/me/player"
#define SPOTIFY_DEVICES_ENDPOINT "/v1/me/player/devices"

#define SPOTIFY_PLAY_ENDPOINT "/v1/me/player/play"
#define SPOTIFY_PAUSE_ENDPOINT "/v1/me/player/pause"
#define SPOTIFY_NEXT_TRACK_ENDPOINT "/v1/me/player/next"

#define SPOTIFY_TOKEN_ENDPOINT "/api/token"


import threading, queue, serial
import datetime 
import subprocess
import time

import serial.tools.list_ports
import requests 
# import osascript
import arduino_secrets

baudRate = 115200
arduinoQueue = queue.Queue()
localQueue = queue.Queue()

refreshToken = "BQCgXZCOuQCbl-8nbJEC_WCgDDg22tHyyVkyytEvyu_iKSgJpkvB4B6qyBjFrESoSeL466J2d1JGHduRdbIG6SNnZI_JSKeWgQk4RMNN9RqFN4DpSBroc9ByxOeapGc6wq2FdyVXZa2X_9gIwo4PqeZEe3YururIi7YJRZk7qzlsBrAkBd1VJOd-a07rHQ"
tokenType = None
ttl = None

SPOTIFY_HOST = "api.spotify.com"
SPOTIFY_ACCOUNTS_HOST = "https://accounts.spotify.com"

SPOTIFY_PLAYER_ENDPOINT = SPOTIFY_HOST + "/v1/me/player"
SPOTIFY_DEVICES_ENDPOINT = SPOTIFY_HOST + "/v1/me/player/devices"

SPOTIFY_PLAY_ENDPOINT = SPOTIFY_HOST + "/v1/me/player/play"
SPOTIFY_PAUSE_ENDPOINT = SPOTIFY_HOST + "/v1/me/player/pause"
SPOTIFY_NEXT_TRACK_ENDPOINT = SPOTIFY_HOST + "/v1/me/player/next"
SPOTIFY_CURRENT_SONG_ENDPOINT = SPOTIFY_HOST + "/v1/me/player/currently-playing"

SPOTIFY_TOKEN_ENDPOINT = SPOTIFY_ACCOUNTS_HOST + "/api/token"

class NoValidPortError(Exception):
    """Exception raised when no valid Arduino ports are found."""
    pass

def selectArduino():
    ports = serial.tools.list_ports.comports()
    valid_ports = [port for port in ports if port.hwid != 'n/a']  # Filter out ports with 'n/a' hwid
    
    if not valid_ports:
        raise NoValidPortError("No valid Arduino ports found.")  # Raise an error if no valid ports

    print('PORT\tDEVICE\t\t\tMANUFACTURER')
    for index, value in enumerate(sorted(valid_ports)):
        print(f"{index}\t{value.name}\t{value.manufacturer}")  # Display sorted list with index

    choice = -1
    while choice < 0 or choice >= len(valid_ports):
        answer = input("➜ Select your port: ")
        if answer.isnumeric():
            choice = int(answer)

    selectedPort = sorted(valid_ports)[choice]  # Map the user's choice to the filtered list
    print(f"selecting: {selectedPort.device}")
    return selectedPort.device


def listenToArduino():
    message = b''
    while True:
        incoming = arduino.read()
        if incoming == b'\n':
            try:
                arduinoQueue.put(message.decode('utf-8').strip())
            except UnicodeDecodeError as e:
                # Handle the error: log it, ignore the message, or take other action
                print(f"UnicodeDecodeError: {e}. Message skipped.")
            message = b''
        else:
            if incoming not in (b'', b'\r'):
                message += incoming

# def listenToLocal():
#     while True:
#         command = sys.stdin.readline().strip().upper()
#         localQueue.put(command)

# def configureUserInput():
#     localThread = threading.Thread(target=listenToLocal, args=())
#     localThread.daemon = True
#     localThread.start()

def configureArduino():
    global arduinoPort
    arduinoPort = selectArduino()
    global arduino
    arduino = serial.Serial(arduinoPort, baudrate=baudRate, timeout=.1)
    arduinoThread = threading.Thread(target=listenToArduino, args=())
    arduinoThread.daemon = True
    arduinoThread.start()

def writeToArduino(message):
    arduino.write(message.encode("utf-8"))
    arduino.write(bytes("\n", encoding="utf-8"))

# def getRefreshToken():
#     payload = f"grant_type=client_credentials&client_id={arduino_secrets.CLIENT_ID}&client_secret={arduino_secrets.CLIENT_SECRET}"
#     headers = {"Content-Type": "application/x-www-form-urlencoded"}
#     r = requests.post(SPOTIFY_TOKEN_ENDPOINT, data=payload, headers=headers)
#     if (r.status_code != 200):
#         writeToArduino("error")
#     refreshToken = r.json()["access_token"]
#     tokenType = r.json()["token_type"]
#     ttl = r.json()["expires_in"]
#     print(f"Refresh Token: {refreshToken}")
    
    # Most likely the Arduino system won't need the refresh token, since only the computer is running requests
    # arduino.write(str(r.json()).encode("utf-8"))
    # arduino.write(bytes("\n", encoding="utf-8"))

# ---- CALLBACKS UPON MESSAGES -----

# def handleLocalMessage(aMessage):
#     print("[LOCAL]: " + aMessage)
#     arduino.write(aMessage.encode('utf-8'))
#     arduino.write(bytes('\n', encoding='utf-8'))
def handlePlay(*args):
    # global refreshToken, tokenType
    # print(f"Using refreshToken: {refreshToken}")
    if (refreshToken is None):
        writeToArduino("error")
    headers = {"Authorization": f"Bearer {refreshToken}", "Content-Type": "application/x-www-form-urlencoded"}

    try:
        r = requests.put("https://api.spotify.com/v1/me/player/play", headers=headers)
        if r.status_code == 204 or r.status_code == 200:
            print(f"{str(datetime.datetime.now())}: [LOCAL] Succesfully started playing.")
            writeToArduino("playback started")
        elif r.status_code in [401, 403]:
            print(f"{str(datetime.datetime.now())}: [LOCAL] Authorization error: {r.status_code}, {r.text}")
            writeToArduino("authorization error")
        elif r.status_code == 429:
            print(f"{str(datetime.datetime.now())}: [LOCAL] Rate limit exceeded")
            writeToArduino("rate limit exceeded")
        else:
            print(f"{str(datetime.datetime.now())}: [LOCAL] Playback failed: {r.status_code}, {r.text}")
            writeToArduino("playback failed")
    except Exception as e:
        print(f"{str(datetime.datetime.now())}: [LOCAL] Error in handlePlay: {e}")
        writeToArduino("error")

def handlePause(*args): 
    if (refreshToken is None):
        writeToArduino("error")
    headers = {"Authorization": f"Bearer {refreshToken}"}

    try:
        r = requests.put("https://api.spotify.com/v1/me/player/pause", headers=headers)
        if r.status_code == 204 or r.status_code == 200:
            print(f"{str(datetime.datetime.now())}: [LOCAL] Successfully paused.")
            writeToArduino("playback started")
        elif r.status_code in [401, 403]:
            print(f"Authorization error: {r.status_code}, {r.text}")
            writeToArduino("authorization error")
        elif r.status_code == 429:
            print(f"{str(datetime.datetime.now())}: [LOCAL] Rate limit exceeded")
            writeToArduino("rate limit exceeded")
        else:
            print(f"{str(datetime.datetime.now())}: [LOCAL] Playback failed: {r.status_code}, {r.text}")
            writeToArduino("playback failed")
    except Exception as e:
        print(f"{str(datetime.datetime.now())}: [LOCAL] Error in handlePlay: {e}")
        writeToArduino("error")

def handleSkip(*args):
    if (refreshToken is None):
        writeToArduino("error")
    headers = {"Authorization": f"Bearer {refreshToken}"}

    try:
        r = requests.post("https://api.spotify.com/v1/me/player/next", headers=headers)
        if r.status_code == 204 or r.status_code == 200:
            print(f"{str(datetime.datetime.now())}: [LOCAL] Successfully skipped.")
            writeToArduino("playback started")
            handleGetSongDuration()
        elif r.status_code in [401, 403]:
            print(f"{str(datetime.datetime.now())}: [LOCAL] Authorization error: {r.status_code}, {r.reason}")
            writeToArduino("authorization error")
        elif r.status_code == 429:
            print(f"{str(datetime.datetime.now())}: [LOCAL] Rate limit exceeded")
            writeToArduino("rate limit exceeded")
        else:
            print(f"{str(datetime.datetime.now())}: [LOCAL] Playback failed: {r.status_code}, {r.reason}")
            writeToArduino("playback failed")
    except Exception as e:
        print(f"{str(datetime.datetime.now())}: [LOCAL] Error in handlePlay: {e}")
        writeToArduino("error")

def restartSong(*args):
    if (refreshToken is None):
        writeToArduino("error")
    headers = {"Authorization": f"Bearer {refreshToken}"}
    r = requests.put("https://api.spotify.com/v1/me/player/seek?position_ms=0", headers=headers)
    if (r.status_code != 204):
        writeToArduino("error")
    elif r.status_code in [401, 403]:
            print(f"Authorization error: {r.status_code}, {r.reason}")
            writeToArduino("authorization error")
    elif r.status_code == 429:
        print("Rate limit exceeded")
        writeToArduino("rate limit exceeded")
    else:
        print(f"Playback failed: {r.status_code}, {r.reason}")
        writeToArduino("playback failed")

def handleGetSongDuration(*args):
    if (refreshToken is None):
        writeToArduino("error")
    headers = {"Authorization": f"Bearer {refreshToken}"}
    r = requests.get("https://api.spotify.com/v1/me/player/currently-playing", headers=headers)
    if (r.status_code != 200):
        print(str(r.status_code) + " " + str(r.reason))
        writeToArduino("error")
    # print(r.json())
    item = r.json()['item']
    if (item is None):
        writeToArduino("error")
    print(f"{str(datetime.datetime.now())}: [LOCAL] Duration: {item['duration_ms']}")
    print(f"{str(datetime.datetime.now())}: [LOCAL] Song name: {item['name']}")
    writeToArduino(str({"duration": item["duration_ms"], "name": item["name"]}))

def handleVolume(*args):
    volume_level = args[0]
    if volume_level.isdigit():
        pot_value = int(volume_level)
            # Ensure pot_value is within the expected range
        if 0 <= pot_value <= 1023:
            # Map potentiometer value (0-1023) to volume range (0.0 to 1.0)
            volume_level = pot_value / 1023.0
            volume_percent = int(volume_level * 100)
            # Use osascript to set the system volume
            try:
                subprocess.run(["osascript", "-e", f"set volume output volume {volume_percent}"])
            except Exception as e:
                print(f"{str(datetime.datetime.now())}: [LOCAL] (Error) {e}")
            print(f"{str(datetime.datetime.now())}: [LOCAL] Volume set to {int(volume_level * 100)}%")
        else:
            print(f"{str(datetime.datetime.now())}: [LOCAL] Ignored out-of-range value '{pot_value}'")
    else:
        print(f"{str(datetime.datetime.now())}: [LOCAL] Ignored non-numeric data '{volume_level}'")

messageResponses = {
    "play": handlePlay,
    "pause": handlePause,
    "skip": handleSkip,
    "getSongDuration": handleGetSongDuration
    # "volume": handleVolume
}

def handleArduinoMessage(aMessage):
    print(str(datetime.datetime.now()) + ": [ARDUINO] " + aMessage)
    tokens = aMessage.strip().split()
    request = tokens[0]
    args = tokens[1:]
    if (request in messageResponses):
        messageResponses[request](*args)

# ---- MAIN CODE -----

configureArduino()                                      # will reboot AVR based Arduinos
# configureUserInput()                                    # handle stdin 

print("Waiting for Arduino")

# --- A good practice would be to wait for a know message from the Arduino
# for example at the end of the setup() the Arduino could send "OK"
while True:
    if not arduinoQueue.empty():
        if arduinoQueue.get() == "OK":
            break
print("Arduino Ready")
# getRefreshToken()
handleGetSongDuration()
restartSong()
handlePause()
# handleGetSongDuration()


# --- Now you handle the commands received either from Arduino or stdin
while True:
    if not arduinoQueue.empty():
        handleArduinoMessage(arduinoQueue.get())

    # if not localQueue.empty():
    #     handleLocalMessage(localQueue.get())