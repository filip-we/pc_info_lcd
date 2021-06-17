import serial
from subprocess import check_output
from time import sleep

baud = 9600
interface = "/dev/ttyACM0"

def send_msg(data):
    try:
        ser = serial.Serial(interface, baud, timeout=4)
        sleep(0.2)
        print(ser.readline())

        ser.write(data.encode("ascii", "ignore"))
        ser.close()
    except serial.serialutil.SerialException:
        print("Could not connect to display - no such USB.")

sensors = check_output("sensors")
sensors = sensors.decode("utf-8").split("\n\n")

gpu = [x for x in sensors[0].split("\n") if "Tctl" in x]
gpu = gpu[0].strip("Tctl:").strip().strip("+")
gpu = gpu.replace(chr(176), " ")

s = ("GPU: " + gpu).ljust(16)
print(s)
send_msg(s)

