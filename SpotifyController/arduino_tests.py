import requests 

SPOTIFY_HOST = "api.spotify.com"
SPOTIFY_ACCOUNTS_HOST = "https://accounts.spotify.com"

SPOTIFY_PLAYER_ENDPOINT = SPOTIFY_HOST + "/v1/me/player"
SPOTIFY_DEVICES_ENDPOINT = SPOTIFY_HOST + "/v1/me/player/devices"

SPOTIFY_PLAY_ENDPOINT = SPOTIFY_HOST + "/v1/me/player/play"
SPOTIFY_PAUSE_ENDPOINT = SPOTIFY_HOST + "/v1/me/player/pause"
SPOTIFY_NEXT_TRACK_ENDPOINT = SPOTIFY_HOST + "/v1/me/player/next"
SPOTIFY_CURRENT_SONG_ENDPOINT = SPOTIFY_HOST + "/v1/me/player/currently-playing"

SPOTIFY_TOKEN_ENDPOINT = SPOTIFY_ACCOUNTS_HOST + "/api/token"

class UnitTests:
    def __init__(self, refreshToken):
        self.headers = {"Authorization": f"Bearer {refreshToken}"}

    def testPlay(self):
        r = requests.put(SPOTIFY_PLAY_ENDPOINT, headers=self.headers)
        assert r.status_code >= 200 and r.status_code < 300

    def testPause(self):
        r = requests.put(SPOTIFY_PAUSE_ENDPOINT, headers=self.headers)
        assert r.status_code >= 200 and r.status_code < 300 

    def testSkip(self):
        r = requests.post(SPOTIFY_NEXT_TRACK_ENDPOINT, headers=self.headers)
        assert r.status_code >= 200 and r.status_code < 300 

    def testGetCurrentSong(self):
        r = requests.get(SPOTIFY_CURRENT_SONG_ENDPOINT, headers=self.headers)
        assert r.status_code == 200
        assert "item" in r.json()
        assert "progress_ms" in r.json() 
        assert "duration_ms" in r.json()["item"]
        assert "name" in r.json()["name"]

    def testComm1(self, write):
        write(str({"PING": "SENDER"}))
        self.testing_comm = True
    
    def testComm2(self, message):
        assert self.testing_comm 
        assert message == "SENDER: PONG"

    def runAll(self, write):
        self.testPlay()
        self.testPause()
        self.testSkip()
        self.testGetCurrentSong()
        self.testComm1(write)