import serial
import struct
from subprocess import check_output
from time import sleep

baud = 9600
port = "/dev/ttyUSB0"

check_output(["stty", "-F", port, "-hupcl"])

def send_msg(data):
    try:
        ser = serial.Serial(None,
                baud,
                timeout=2,
                writeTimeout=2,
                )

        sleep(0.2)
        ser.port = port

        ser.open()
        #print("Reading...")
        #print(ser.read(64))

        print("Writing...")
        print(data)
        ser.write(data)

        print("Closing serial...")
        ser.close()
    except serial.serialutil.SerialException:
        print("Could not connect to display - no such device.")
        ser.close()
    print("Return")


def get_gpu_temp():
    sensors = check_output("sensors")
    sensors = sensors.decode("utf-8").split("\n\n")

    gpu = [x for x in sensors[0].split("\n") if "Tctl" in x]
    gpu = gpu[0].strip("Tctl:").strip().strip("+")
    gpu = gpu.replace(chr(176) + "C", "")

    gpu = float(gpu)
    return bytearray(struct.pack("<f", gpu)) + bytearray(3)

def get_cpu_status():
    sensors = check_output("sensors")
    sensors = sensors.decode("utf-8").split("\n\n")
    print(sensors)


data = bytearray(b'\x11') + get_gpu_temp()

send_msg(data)

get_cpu_status()
