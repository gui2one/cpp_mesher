import socket
import time
from enum import StrEnum

class COMMAND(StrEnum) :
    LOAD_MESH = "LOAD_MESH"
    MESH_FILE_UPDATE = "MESH_FILE_UPDATE"
    MESH_FILE_UPDATE_BROADCAST = "MESH_FILE_UPDATE_BROADCAST"
    GREET = "GREET"

HOST = "127.0.0.1"
PORT = 61725

def send_command(cmd : COMMAND):
    with socket.socket(socket.AF_INET,socket.SOCK_STREAM) as s :
        try :
            s.connect((HOST, PORT))
            print(f"send_command() for {cmd} ...")
            s.sendall(cmd.encode())
        except ConnectionRefusedError: 
            print(f"failed to connect to {HOST}") 

def wait_for_update():
    print("Waiting for an update ...")
    with socket.socket(socket.AF_INET,socket.SOCK_STREAM) as s :
        try :
            s.connect((HOST, PORT))

            data = bytearray()
            while True:
                packet = s.recv(4096)
                if not packet :
                    break
                data.extend(packet)
            print(data.decode())                
        except ConnectionRefusedError: 
            print(f"failed to connect to {HOST}")
        except : 
            pass
        
def get_reponse():
    with socket.socket(socket.AF_INET,socket.SOCK_STREAM) as s :
        try:
            s.connect((HOST, PORT))

            data = bytearray()
            while True:
                packet = s.recv(4096)
                if not packet :
                    break
                data.extend(packet)
            return data
        except ConnectionRefusedError: 
            print(f"failed to connect to {HOST}")

def receive_mesh():
        data = get_reponse()
        print(data.decode())


send_command(COMMAND.MESH_FILE_UPDATE_BROADCAST)
wait_for_update()

send_command(COMMAND.LOAD_MESH)
receive_mesh()

send_command(COMMAND.GREET)
res = get_reponse()
print(res.decode())

    

#     # s.close()
