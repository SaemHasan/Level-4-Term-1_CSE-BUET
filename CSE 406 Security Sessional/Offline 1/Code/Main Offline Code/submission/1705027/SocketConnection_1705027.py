import pickle
class SocketConnection:
    def __init__(self, con):
        self.con = con
        self.bufferSize = 1024
    
    def send(self, data):
        data = pickle.dumps(data)
        self.con.send(str(len(data)).encode())
        reply = self.con.recv(1024).decode()
        if reply == 'ack':
            for i in range(0, len(str(data)), self.bufferSize):
                self.con.send(data[i:i+self.bufferSize])

    def recieve(self):
        length = self.con.recv(1024).decode()
        self.con.send('ack'.encode())
        data = bytes()
        while len(data) < int(length):
            data += self.con.recv(self.bufferSize)
        return pickle.loads(data)

