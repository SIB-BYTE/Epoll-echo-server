# Epoll-echo-server
This is a TCP-based server that handles non-blocking I/O with epoll in C.

Usage:
```
make
./server IP PORT

nc IP PORT
```

upon the netcat connection to the server, you can send data and it will echo it back to the netcat connection. Pretty useless but I need to upload SOMETHING.
