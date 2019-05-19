import socket
import json

# Create the register packet
register_packet = {
    "username": "almog",
    "password": "SomeStrongPassword",
    "email": "someMail@mailService.com"
}

# Convert the register packet to json str
register_packet_serialized = json.dumps(register_packet).encode()

# Create the buffer structure
send_buf = bytearray(1)
print(len(register_packet_serialized))

send_buf[0] = 1 # Request id
send_buf += len(register_packet_serialized).to_bytes(4, byteorder='little')

print(send_buf)

# Create a TCP socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect to the local server
s.connect(("localhost", 61110))

# Send the register packet to the server
s.send(send_buf)
s.send(register_packet_serialized)

# Get result from server
res = s.recv(1024)
print(res)