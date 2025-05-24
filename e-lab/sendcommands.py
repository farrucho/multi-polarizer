import serial
import time

# Serial port configuration
COM_PORT = '/dev/ttyACM0'  # Replace with your Arduino's port (e.g., "COM3" on Windows or "/dev/ttyUSB0" on Linux/Mac)
BAUD_RATE = 115200  # Match the Arduino's baud rate

pic = serial.Serial(COM_PORT, BAUD_RATE, timeout=1)

pic.write('cfg \t 1\t 0\t 0\t 0\t 0\t 0\t 1\t 900 \t 255\r'.encode())
# pic.write('rst\r'.encode())
# pic.write('cfg \t 0\t 1\t 2\t 3\t 4\t 5\t 1\t 900 \r'.encode())
# pic.write('str\r'.encode())
