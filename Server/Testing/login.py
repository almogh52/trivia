import socket
import json

# Create the login packet
login_packet = {
    "username": "almog",
    "password": "SomeStrongPassword"
}

# Convert the login packet to json str
login_packet_serialized = json.dumps(login_packet).encode()

# Create the buffer structure
send_buf = bytearray(1)
print(len(login_packet_serialized))

send_buf[0] = 0 # Request id
send_buf += len(login_packet_serialized).to_bytes(4, byteorder='little')

print(send_buf)

# Create a TCP socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect to the local server
s.connect(("localhost", 61110))
print("Connected")

# Send the login packet to the server
s.send(send_buf)
s.send(login_packet_serialized)
print("Sent request")

# Get result size from server
msg_size = int.from_bytes(s.recv(4), byteorder="little")

# Get result
res = s.recv(msg_size)
print(res)