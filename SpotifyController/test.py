import serial
import serial.tools.list_ports

# List available ports
ports = serial.tools.list_ports.comports()
print("Available Ports:")
for port in ports:
    print(port.device)

# Connect to a specific port
port = '/dev/cu.usbmodemF412FA9F08242'  # Replace with your port
baud_rate = 115200

try:
    arduino = serial.Serial(port, baud_rate)
    print(f"Connected to {port}")
except Exception as e:
    print(f"Error: {e}")