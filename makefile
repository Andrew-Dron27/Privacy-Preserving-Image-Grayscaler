CC = g++
CFLAGS = -Wall -g

SERVER_SRC_DIR := ./src/server
CLIENT_SRC_DIR := ./src/client
BIN_DIR     = ./bin
 
# ****************************************************
# Targets needed to bring the executable up to date

# Object files
OBJS = $(SRCS:.cpp=.o)

SERVER_SRCS := $(SERVER_SRC_DIR)/server.cpp \
		$(SERVER_SRC_DIR)/server.h \

# Object files
SERVER_OBJS = $(SERVER_SRCS:.cpp=.o)

# Object files
CLIENT_OBJS = $(CLIENT_SRCS:.cpp=.o)	 

$(OBJ_DIR) $(BIN_DIR):
	mkdir -p $@

server: $(SERVER_SRC_DIR)/server.o
	$(CC) $(CFLAGS) -o $(BIN_DIR)/server $(SERVER_SRC_DIR)/server.o

client: $(CLIENT_SRC_DIR)/client.o
	$(CC) $(CFLAGS) -o $(BIN_DIR)/client $(CLIENT_SRC_DIR)/client.o
 
# The main.o target can be written more simply
 
server.o: $(SERVER_SRC_DIR)/server.cpp $(SERVER_SRC_DIR)/server.h
	$(CC) $(CFLAGS) -c $(SERVER_SRC_DIR)/server.h
 
client.o: $(CLIENT_SRC_DIR)/client.cpp $(CLIENT_SRC_DIR)/client.h
	$(CC) $(CFLAGS) -c $(CLIENT_SRC_DIR)/client.h

