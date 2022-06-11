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
	int servfd = setup_socket(socket(AF_INET, SOCK_STREAM, 0), argv[1], atoi(argv[2]));
	setup_epoll(servfd);
}

// Setup socket creation.
int setup_socket(int fd, char *address, short port)
{
	struct sockaddr_in sock = {
		.sin_family		 = AF_INET,
		.sin_port		 = htons(port),
		.sin_addr.s_addr = inet_addr(address)
	};

	socklen_t socklen = sizeof(sock);

	LASSERT(bind(fd, (struct sockaddr *)&sock, socklen) != -1, "Binding errors -> {%s : %d}\n", strerror(errno), errno);
	printf("<%s> binded  <%d>\n", address, port);

	LASSERT(listen(fd, SOMAXCONN) != -1, "Listening errors -> {%s : %d}\n", strerror(errno), errno);
	printf("<%s> backlog <%d>\n", address, SOMAXCONN);

	//set_nonblocking(fd);

	return (fd);
}

// Handle socket stream data.
void handle_data(int fd)
{
	char buffer[50] = {0};
	ssize_t read_return = read(fd, buffer, sizeof(buffer));

	if(!read_return)
	{
		printf("[FD : %d] disconnected\n", fd);
		close(fd);

		return;
	}
	else if((read_return == -1) && (errno != EAGAIN || errno != EWOULDBLOCK))
		perror("read() error");

	printf("[FD : %d] message to be echoed -> %s\n", fd, buffer);
	write(fd, buffer, strlen(buffer));
}

/*
void register_user(int fd, user_t *user)
{
	// Handle user state, and time active.
}
*/

void setup_epoll(int fd)
{
	int epfd = epoll_create1(0);
	LASSERT(epfd != -1, "Epoll errors -> {%s : %d}\n", strerror(errno), errno);

	struct epoll_event event = {
		.events   = EPOLLIN,
		.data.fd  = fd
	};

	LASSERT(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event) != -1, "epoll_ctl() errors -> {%s : %d}\n", strerror(errno), errno);

	struct epoll_event *events = calloc(MAX_EVENTS, sizeof(struct epoll_event));

	for(;;)
	{
		int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
		LASSERT(nfds != -1, "Epoll_wait() errors -> {%s : %d}\n", strerror(errno), errno);

		for(int i = 0; i < nfds; ++i)
		{
			if(events[i].data.fd == fd)
			{
      		struct sockaddr sock = {0};
				  socklen_t socklen = sizeof(sock);

  				int cfd = accept(fd, &sock, &socklen);
				  if(cfd == -1) break;

      		event.data.fd = cfd;
      		event.events = EPOLLIN;

				  printf("[FD : %d] has joined!\n", cfd);

          set_nonblocking(cfd);

			   	if(epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &event) == -1)
				  {
					  printf("<FD : %d> disconnected!", fd);
					  close(cfd);
				  }
			 }
			 else
			 {
			  	handle_data(events[i].data.fd);
			 }
		}
	}
}

