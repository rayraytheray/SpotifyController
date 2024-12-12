import serial
import subprocess
import threading

class AudioController:
    def __init__(self, serial_port, baud_rate=115200, threshold=10):
        self.serial_port = serial_port
        self.baud_rate = baud_rate
        self.threshold = threshold
        self.last_pot_value = None
        self.running = True

    def set_volume(self, volume_level):
        volume_percent = int(volume_level * 100)
        command = ["osascript", "-e", f"set volume output volume {volume_percent}"]
        try:
            subprocess.run(command, check=True)
            print(f"Volume set to: {volume_percent}%")
        except subprocess.CalledProcessError as e:
            print(f"Error setting volume: {e}")

    def clean_data(self, data):
        return ''.join(filter(str.isdigit, data))

    def run(self):
        try:
            with serial.Serial(self.serial_port, self.baud_rate, timeout=1) as arduino:
                while self.running:
                    try:
                        raw_data = arduino.readline().decode('utf-8', errors='ignore').strip()
                        if not raw_data:
                            continue

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
        except Exception as e:
            print(f"Error opening serial port: {e}")

    def stop(self):
        self.running = False

def start_audio_controller(port):
    audio_controller = AudioController(port)
    audio_thread = threading.Thread(target=audio_controller.run, daemon=True)
    audio_thread.start()
    return audio_controller


# import serial
# import subprocess

# # Set up Arduino serial port
# arduino = serial.Serial('/dev/cu.usbmodemF412FA9F08242', 115200)

# def set_volume(volume_level):
#     # Convert volume level to a percentage (0 to 100)
#     volume_percent = int(volume_level * 100)
#     command = ["osascript", "-e", f"set volume output volume {volume_percent}"]
#     print(f"Running command: {' '.join(command)}")  # Debugging output

#     try:
#         subprocess.run(command, check=True)
#         print(f"Volume successfully set to: {volume_percent}%")
#     except subprocess.CalledProcessError as e:
#         print(f"Error setting volume: {e}")


# def clean_data(data):
#     """
#     Cleans the raw data by removing non-numeric characters.
#     """
#     return ''.join(filter(str.isdigit, data))

# last_pot_value = None
# threshold = 10  # Minimum change to trigger an update

# while True:
#     try:
#         # Read raw data
#         raw_data = arduino.readline().decode('utf-8', errors='ignore').strip()
#         if not raw_data:
#             print("Received empty data")
#             continue

#         print(f"Raw data: {raw_data}")

#         # Clean and process the data
#         clean_raw_data = clean_data(raw_data)
#         if clean_raw_data.isdigit():
#             pot_value = int(clean_raw_data)
#             if 0 <= pot_value <= 1023:
#                 # Update volume only if the change exceeds the threshold
#                 if last_pot_value is None or abs(pot_value - last_pot_value) > threshold:
#                     volume_level = pot_value / 1023.0
#                     set_volume(volume_level)
#                     last_pot_value = pot_value
#             else:
#                 print(f"Ignored out-of-range value: {pot_value}")
#         else:
#             print(f"Ignored non-numeric data: {raw_data}")
#     except Exception as e:
#         print(f"Error: {e}")
