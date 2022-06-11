# Compiler:
CC   = gcc
LLVM = clang

# Server setup:
SERVER_BUILD = -Wall -Wextra -Wpedantic -Og -o
SERVER_DEBUG = -Wall -Wextra -Wpedantic -fsanitize=address,undefined -g3 -Og -D DEBUG -o
SERVER_BIN = server

SERVER_SRC = server.c

# Client setup:
CLIENT_BUILD = -Wall -Wextra -Wpedantic -Og -o
CLIENT_DEBUG = -Wall -Wextra -Wpedantic -fsanitize=address,undefined -g3 -Og -DEBUG -o
CLIENT_BIN = client

CLIENT_SRC = client.c

# Builds:
server_build:
	$(CC) $(SERVER_SRC) $(SERVER_BUILD) $(SERVER_BIN)

server_debug:
	$(CC) $(SERVER_SRC) $(SERVER_DEBUG) $(SERVER_BIN)

client_build:
	$(CC) $(CLIENT_SRC) $(CLIENT_BUILD) $(CLIENT_BIN)

client_debug:
	$(CC) $(CLIENT_SRC) $(CLIENT_DEBUG) $(CLIENT_BIN)
