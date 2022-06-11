#include "server.h"

/*
File layout:
 * (Top)    - Main function.
 * (Middle) - Socket setup and data handling.
 * (Bottom) - Epoll setup and handling.
*/

int main(int argc, char **argv)
{
	LASSERT(argc == 3, "Usage: %s <IP> <PORT>\n", argv[0]);

	// Returns the servers file descriptor.
	int servfd = setup_socket(socket(AF_INET, SOCK_STREAM, 0), argv[1], atoi(argv[2]));
	setup_epoll(servfd);
}

// Setup socket creation.
int setup_socket(int fd, char *address, short port)
{
	// Sets up the context for the server socket.
	struct sockaddr_in sock = {
		.sin_family	 = AF_INET,
		.sin_port	 = htons(port),
		.sin_addr.s_addr = inet_addr(address)
	};

	socklen_t socklen = sizeof(sock);

	// Binds the socket to the port.
	LASSERT(bind(fd, (struct sockaddr *)&sock, socklen) != -1, "Binding errors -> {%s : %d}\n", strerror(errno), errno);
	printf("<%s> binded  <%d>\n", address, port);

	// Listens for a backlog of a max of 1024 connections.
	LASSERT(listen(fd, SOMAXCONN) != -1, "Listening errors -> {%s : %d}\n", strerror(errno), errno);
	printf("<%s> backlog <%d>\n", address, SOMAXCONN);

	// Sets the server to be non-blocking.
	set_nonblocking(fd);

	return (fd);
}

// Handle socket stream data.
void handle_data(int fd)
{
	char buffer[50] = {0};
	ssize_t read_return = read(fd, buffer, sizeof(buffer));

	// Checks if a file descriptor leaves.
	if(!read_return)
	{
		printf("[FD : %d] disconnected\n", fd);
		close(fd);

		return;
	}
	// Checks to see if there's a read() error.
	else if((read_return == -1) && (errno != EAGAIN || errno != EWOULDBLOCK))
		perror("read() error");

	// Announces the data and writes it back to the accepted file descriptor.
	printf("[FD : %d] message to be echoed -> %s\n", fd, buffer);
	write(fd, buffer, strlen(buffer));
}

void setup_epoll(int fd)
{
	int epfd = epoll_create1(0);
	LASSERT(epfd != -1, "Epoll errors -> {%s : %d}\n", strerror(errno), errno);

	struct epoll_event event = {
		.events   = EPOLLIN, // Level-triggering epoll interface.
		.data.fd  = fd
	};

	// Adds the server file descriptor to the epoll watch list.
	LASSERT(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event) != -1, "epoll_ctl() errors -> {%s : %d}\n", strerror(errno), errno);

	// Creates a dynamic array of structures for each individual accepted connection.
	struct epoll_event *events = calloc(MAX_EVENTS, sizeof(struct epoll_event));

	for(;;)
	{
		// Returns the number of events that have happened.
		int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
		LASSERT(nfds != -1, "Epoll_wait() errors -> {%s : %d}\n", strerror(errno), errno);

		// Iterates over the number of events that are populated in "events" which is directly linked to the "epoll_wait" call.
		for(int i = 0; i < nfds; ++i)
		{
			// If the currenly accepted event is the server file descriptor, that means there's an incoming connection and we must handle it.
			if(events[i].data.fd == fd)
			{
  				int cfd = accept(fd, NULL, NULL);
				if(cfd == -1) break;
				
				// Set the context of the epoll structure even to handle this specific accepted file descriptor.
      				event.data.fd = cfd;
      				event.events = EPOLLIN;

				// Announce it's connection.
				printf("[FD : %d] has joined!\n", cfd);

				// Make it non-blocking.
          			set_nonblocking(cfd);

				// Add the accepted incoming connection to the epoll watch-list.
			   	if(epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &event) == -1)
				{
					printf("<FD : %d> disconnected!", fd);
					close(cfd);
				}
			 }
			 // Not the server file descriptor, meaning this is an accepted connection which we will handle separately.
			 else
			 {
			  	handle_data(events[i].data.fd);
			 }
		}
	}
}

