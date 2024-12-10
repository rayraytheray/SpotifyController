import serial
import subprocess

# Set up Arduino serial port
arduino = serial.Serial('COM6', 115200)  # Match baud rate with Arduino

def set_volume(volume_level):
    # Convert volume to a percentage (0 to 100)
    volume_percent = int(volume_level * 100)
    # Use osascript to set the system volume
    subprocess.run(["osascript", "-e", f"set volume output volume {volume_percent}"])

def clean_data(data):
    """
    Cleans the raw data by removing non-numeric characters and ensuring
    it can be safely converted to an integer.
    """
    return ''.join(filter(str.isdigit, data))

while True:
    try:
        # Read raw data from Arduino
        raw_data = arduino.readline().decode('utf-8', errors='ignore').strip()
        print(f"Raw data: {raw_data}")  # Debugging step

        # Clean the data
        clean_raw_data = clean_data(raw_data)

        # Check if the cleaned data is numeric
        if clean_raw_data.isdigit():
            pot_value = int(clean_raw_data)

            # Ensure pot_value is within the expected range
            if 0 <= pot_value <= 1023:
                # Map potentiometer value (0-1023) to volume range (0.0 to 1.0)
                volume_level = pot_value / 1023.0
                set_volume(volume_level)
                print(f"Volume set to: {int(volume_level * 100)}%")
            else:
                print(f"Ignored out-of-range value: {pot_value}")
        else:
            print(f"Ignored non-numeric data: {raw_data}")

    except Exception as e:
        print(f"Error: {e}")

# import serial
# import subprocess

# # Set up Arduino serial port
# arduino = serial.Serial('/dev/cu.usbmodemF412FA9F08242', 9600)

# def set_volume(volume_level):
#     # Convert volume to a percentage (0 to 100)
#     volume_percent = int(volume_level * 100)
#     # Use osascript to set the system volume
#     subprocess.run(["osascript", "-e", f"set volume output volume {volume_percent}"])

# while True:
#     try:
#         # Read raw data from Arduino
#         raw_data = arduino.readline().decode().strip()
#         print(f"Raw data: {raw_data}")  # Debugging step

#         # Ensure data is numeric
#         if raw_data.isdigit():
#             pot_value = int(raw_data)

#             # Map potentiometer value (0-1023) to volume range (0.0 to 1.0)
#             volume_level = pot_value / 1023.0
#             set_volume(volume_level)
#             print(f"Volume set to: {int(volume_level * 100)}%")
#         else:
#             print(f"Ignored non-numeric data: {raw_data}")

#     except Exception as e:
#         print(f"Error: {e}")



# import serial
# import subprocess

# # Set up Arduino serial port
# arduino = serial.Serial('/dev/cu.usbmodemF412FA9F08242', 9600)

# def set_volume(volume_level):
#     # Convert volume to a percentage (0 to 100)
#     volume_percent = int(volume_level * 100)
#     # Use osascript to set the system volume
#     subprocess.run(["osascript", "-e", f"set volume output volume {volume_percent}"])

# while True:
#     try:
#         # Read value from Arduino
#         data = arduino.readline().decode().strip()
#         pot_value = int(data)

#         # Map potentiometer value (0-1023) to volume range (0.0 to 1.0)
#         volume_level = pot_value / 1023.0
#         set_volume(volume_level)
#         print(f"Volume set to: {int(volume_level * 100)}%")
#     except Exception as e:
#         print(f"Error: {e}")

# import serial
# from ctypes import cast, POINTER
# from comtypes import CLSCTX_ALL
# from pycaw.pycaw import AudioUtilities, IAudioEndpointVolume

# # Set up Arduino serial port
# arduino = serial.Serial('/dev/cu.usbmodemF412FA9F08242', 9600)  

# # Initialize Pycaw for volume control
# devices = AudioUtilities.GetSpeakers()
# interface = devices.Activate(
#     IAudioEndpointVolume._iid_, CLSCTX_ALL, None)
# volume = cast(interface, POINTER(IAudioEndpointVolume))

# while True:
#     try:
#         # Read value from Arduino
#         data = arduino.readline().decode().strip()
#         pot_value = int(data)

#         # Map potentiometer value (0-1023) to volume range (0.0 to 1.0)
#         volume_level = pot_value / 1023.0
#         volume.SetMasterVolumeLevelScalar(volume_level, None)
#         print(f"Volume set to: {int(volume_level * 100)}%")
#     except Exception as e:
#         print(f"Error: {e}")