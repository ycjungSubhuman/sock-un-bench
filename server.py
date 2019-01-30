import socket
import sys
import os
import time

sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
sock.bind('./test')

sock.listen(1)
print ('setup')

conn, _ = sock.accept()
t = time.time()
print ('conn established')
cnt = 0
while True:
    conn.sendall(b'give\0')
    for i in range(0,256):
        d = conn.recv(4096)
        assert(d)
    cnt += 1
    if time.time() - t >= 10.0:
        print('Time up')
        conn.sendall(b'done\0')
        break
    conn.sendall(b'good\0')
print ('{} images in 10 sec'.format(cnt))
time.sleep(1)

