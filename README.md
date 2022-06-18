# Epoll-echo-server
This is a TCP-based server that handles non-blocking I/O with epoll in C.

# Usage:
```
make
./server IP PORT

nc IP PORT
```

upon the netcat connection to the server, you can send data and it will echo it back to the netcat connection. Pretty useless but I need to upload 
SOMETHING.

# What is epoll?
Epoll is a linux system call that's used to monitor and handle multiplexing I/O && non-blocking I/O from a list of file descriptors, it's incredibly useful for handling a lot of file descriptors for web servers, TCP servers and more. Epoll is much newer but superior in every way to it's previous implementations such as ``select`` and ``poll``, epoll doesn't use linear polling to find file descriptors, it uses red-black binary search trees to preform constant time look ups. It also has the option to have 2 different event notification interfaces; which are level and edge triggering rather than forcing 1 type of interface.

# Resources:
Man pages        : https://man7.org/linux/man-pages/man7/epoll.7.html

Setting up epoll : https://suchprogramming.com/epoll-in-3-easy-steps/

Stackoverflow thread for resources : https://stackoverflow.com/questions/27247/could-you-recommend-some-guides-about-epoll-on-linux
