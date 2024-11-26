import struct
import serial
import time
import random
import ctypes


data_definitions = {
    "1": ("OIL PRESSURE", 0, 1000),
    "2": ("OIL TEMPERATURE", 0, 400),
    "3": ("FUEL FLOW", 0, 800),
    "4": ("FUEL", 0, 800),
    "5": ("EGT", 0, 400),
    "6": ("TORQUE", 0, 400),
    "7": ("INDICATED POWER", 0, 400),
    "8": ("Frictional Power", 0, 400),
    "9": ("Thermal efficiency", 0, 100),
    "0A": ("Air-Fuel ratio", 0, 20),
    "0B": ("MOTOR SPEED", 0, 1000),
    "0C": ("OUTPUT AIR SPEED", 0, 1000),
    "0D": ("VIBRATION", 0, 100),
    "0E": ("BODY TEMP", 0, 400),
    "0F": ("AIR TEMP", 0, 400),
    "11": ("OIL PRESSURE SENSOR ERROR(0=OK, 1=ERROR)", 0, 1),
    "12": ("OIL TEMPERATURE SENSOR ERROR", 0, 1),
    "13": ("FUEL FLOW SENSOR ERROR", 0, 1),
    "14": ("FUEL SENSOR ERROR", 0, 1),
    "15": ("EGT SENSOR ERROR", 0, 1),
    "16": ("TORQUE SENSOR ERROR", 0, 1),
    "17": ("INDECATED POWER SENSOR ERROR", 0, 1),
    "18": ("Frictional Power SENSOR ERROR", 0, 1),
    "19": ("Thermal efficiency SENSOR ERROR", 0, 1),
    "1A": ("Air-Fuel ratio SENSOR ERROR", 0, 1),
    "1B": ("MOTOR SPEED SENSOR ERROR", 0, 1),
    "1C": ("OUTPUT AIR SPEED SENSOR ERROR", 0, 1),
    "1D": ("VIBRATION SENSOR ERROR", 0, 1),
    "1E": ("BODY TEMP SENSOR ERROR", 0, 1),
    "1F": ("AIR TEMP SENSOR ERROR", 0, 1),
}


def calculate_checksum(data):
    checksum = ctypes.c_uint16(0)
    for byte in data:
        checksum.value += byte
    return struct.pack(">H", checksum.value)


def generateDATA(id_i, data, factor):
    reserve = [0x00]
    data_bytes = list(struct.pack(">I", data))
    factor_bytes = list(struct.pack(">I", factor))
    message = [id_i] + reserve + data_bytes + factor_bytes
    return message


# Configure the serial port
ser = serial.Serial(
    port="/home/danial/4",  # change it for to your port
    baudrate=115200,
    parity=serial.PARITY_ODD,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    # timeout=1,
)


def generate_random_message():
    header = [0xA5, 0xA5, 0xA5, 0xA5]
    msg_counter = [random.randint(0, 255)]
    # id_number = [random.randint(1, 31)]  # Random ID number between 1 and 31/
    id_number = [30]
    id_message = []

    for i in range(1, id_number[0] + 1):
        id_i = random.randint(0, 255)
        id_i = i
        if id_i == 16:
            id_i = 31

        data = random.randint(200, 600)
        factor = random.randint(0, 1)
        id_message.append(generateDATA(id_i, data, factor))

    # Flatten id_message list
    flattened_id_message = (
        msg_counter + id_number + [item for sublist in id_message for item in sublist]
    )
    # Calculate checksum for the flattened id_message
    checksum = list(calculate_checksum(flattened_id_message))
    footer = [0x55]

    # Convert header, msg_counter, id_number, and checksum to hex strings
    header_str = "".join(format(x, "02X") for x in header)
    id_message_str = "".join(format(x, "02X") for x in flattened_id_message)
    checksum_str = "".join(format(x, "02X") for x in checksum)
    footer_str = "".join(format(x, "02X") for x in footer)

    # Append the final message
    final_message = header_str + id_message_str + checksum_str + footer_str

    return bytes.fromhex(final_message)


try:
    while True:
        message = generate_random_message()
        ser.write(message)
        print(f"Sent: {message.hex().upper()}")
        # change the interval time for sending the message
        time.sleep(1)

except KeyboardInterrupt:
    print("Stopped by user")

finally:
    ser.close()
