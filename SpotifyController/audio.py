import serial
import subprocess
import threading
"""
The AudioController class is responsible for controlling the host computer volume based on data received 
from our Arduino potentiometer connected through the serial port. The class listens to serial input, cleans the 
data, and adjusts the volume if the input value meets specified threshold.
"""
class AudioController:
    def __init__(self, serial_port, baud_rate=115200, threshold=10):
        """
        Initializes the AudioController connecting the serial port.
        """
        self.serial_port = serial_port
        self.baud_rate = baud_rate
        self.threshold = threshold
        self.last_pot_value = None
        self.running = True

    def set_volume(self, volume_level):
        """
        Sets the host computer volume
        """
        volume_percent = int(volume_level * 100)
        command = ["osascript", "-e", f"set volume output volume {volume_percent}"]
        try:
            subprocess.run(command, check=True)
            print(f"Volume set to: {volume_percent}%")
        except subprocess.CalledProcessError as e:
            print(f"Error setting volume: {e}")

    def clean_data(self, data):
        """
        Removes any non-numeric characters from our raw data to prevent errors converting volume
        """
        return ''.join(filter(str.isdigit, data))
    def run(self, max_iterations=None):
        """
        Begins listening tot he serial input and adjusts the volume if the threshold is met
        """
        iteration_count = 0
        try:
            with serial.Serial(self.serial_port, self.baud_rate, timeout=1) as arduino:
                while self.running:
                    if max_iterations is not None and iteration_count >= max_iterations:
                        break  
                    
                    try:
                        raw_data = arduino.readline().decode('utf-8', errors='ignore').strip()
                        if not raw_data:
                            break  

                        clean_raw_data = self.clean_data(raw_data)
                        if clean_raw_data.isdigit():
                            pot_value = int(clean_raw_data)
                            if 0 <= pot_value <= 1023:
                                if self.last_pot_value is None or abs(pot_value - self.last_pot_value) > self.threshold:
                                    volume_level = pot_value / 1023.0
                                    self.set_volume(volume_level)
                                    self.last_pot_value = pot_value
                    except Exception as e:
                        print(f"Error processing data: {e}")
                    iteration_count += 1
        except Exception as e:
            print(f"Error opening serial port: {e}")
    def stop(self):
        self.running = False

def start_audio_controller(port):
    """
    Creates the AudioController in a new thread. 
    """
    audio_controller = AudioController(port)
    audio_thread = threading.Thread(target=audio_controller.run, daemon=True)
    audio_thread.start()
    return audio_controller

