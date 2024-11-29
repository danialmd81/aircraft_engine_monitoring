import struct
import serial
import time
import random
import ctypes

other_data_definitions = {
    "01": ("OIL PRESSURE", 0, 1000),
    "02": ("OIL TEMPERATURE", 0, 400),
    "03": ("FUEL FLOW", 0, 800),
    "04": ("FUEL", 0, 800),
    "05": ("EGT", 0, 400),
    "06": ("TORQUE", 0, 400),
    "07": ("INDICATED POWER", 0, 400),
    "08": ("Frictional Power", 0, 400),
    "09": ("Thermal efficiency", 0, 100),
    "0A": ("Air-Fuel ratio", 0, 20),
    "0B": ("MOTOR SPEED", 0, 1000),
    "0C": ("OUTPUT AIR SPEED", 0, 1000),
    "0D": ("VIBRATION", 0, 100),
    "0E": ("BODY TEMP", 0, 400),
    "0F": ("AIR TEMP", 0, 400),
}

error_data_definitions = {
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
    data_bytes = list(struct.pack("<I", data))
    factor_bytes = list(struct.pack("<I", factor))
    print(f"Data: {data}, 0xData : {''.join(format(x, '02X') for x in data_bytes)}")

    print(
        f"Factor: {factor}, 0xFactor : {''.join(format(x, '02X') for x in factor_bytes)}"
    )
    message = [id_i] + reserve + data_bytes + factor_bytes
    return message


# # Configure the serial port
# try:
#     ser = serial.Serial(
#         port="/home/danial/4",  # change it for to your port
#         baudrate=115200,
#         parity=serial.PARITY_ODD,
#         stopbits=serial.STOPBITS_ONE,
#         bytesize=serial.EIGHTBITS,
#         timeout=1,
#     )
# except serial.SerialException as e:
#     print(f"Error opening serial port: {e}")
#     exit(1)
# Configure the serial port
try:
    ser = serial.Serial(
        port="COM2",  # change it for to your port
        baudrate=115200,
        parity=serial.PARITY_ODD,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout=1,
    )
except serial.SerialException as e:
    print(f"Error opening serial port: {e}")
    exit(1)


def generate_random_message():
    header = [0xA5, 0xA5, 0xA5, 0xA5]
    msg_counter = [random.randint(0, 255)]
    id_number = [30]
    # id_number = [2]
    id_message = []
    for i in range(1, id_number[0] + 1):
        id_i = random.randint(1, 31)
        if id_i == 16:
            id_i = 31

        id_hex = (
            format(id_i, "X").upper().zfill(2)
        )  # Convert id_i to two-character hexadecimal string

        if id_i <= 15:  # Other data
            data_range = other_data_definitions[id_hex]
            data = (
                random.randint(data_range[1], data_range[2]) + data_range[2]
                if random.random() < 0.1
                else random.randint(data_range[1], data_range[2])
            )  # 10% chance of error data
            factor = (
                random.randint(0, 1000) if random.random() < 0.4 else 0
            )  # generate float number for other data by 40% chance
        else:  # Error data
            data_range = error_data_definitions[id_hex]
            data = 1 if random.random() < 0.1 else 0  # 10% chance of error
            factor = (
                random.randint(0, 1000) if random.random() < 0.1 else 0
            )  # generate float number for error data by 10% chance

        id_message.append(generateDATA(id_i, data, factor))

    # Flatten id_message list
    flattened_id_message = (
        msg_counter + id_number + [item for sublist in id_message for item in sublist]
    )
    # Calculate checksum for the flattened id_message
    checksum = list(calculate_checksum(flattened_id_message))
    checksum = checksum[::-1]  # Reverse the checksum bytes for little endian order
    footer = [0x55]

    # Convert header, msg_counter, id_number, and checksum to hex strings
    header_str = "".join(format(x, "02X") for x in header)
    id_message_str = "".join(format(x, "02X") for x in flattened_id_message)
    checksum_str = "".join(format(x, "02X") for x in checksum)
    footer_str = "".join(format(x, "02X") for x in footer)

    # Append the final message
    final_message = header_str + id_message_str + checksum_str + footer_str

    return bytes.fromhex(final_message)


def generate_garbage_data():
    length = random.randint(10, 50)
    return bytes([random.randint(0, 255) for _ in range(length)])


try:
    while True:
        if random.random() < 0.2:  # 20% chance to send garbage data
            message = generate_garbage_data()
            print(f"Sent garbage data: {message.hex().upper()}")
        else:
            message = generate_random_message()
            print(f"Sent: {message.hex().upper()}")
        ser.write(message)
        # Sleep for a random time between 0.02 and 1 seconds
        time.sleep(random.uniform(0.02, 1.0))
        # time.sleep(random.uniform(0.1, 1.0))
        # time.sleep(2)

except KeyboardInterrupt:
    ser.close()
    print("Stopped by user")
