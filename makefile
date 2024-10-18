CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g

SRCDIR = src
OBJDIR = obj
BINDIR = bin

SERVER_SRC_DIR := ./src/server
SERVER_OBJ_DIR := ./obj/server
CLIENT_SRC_DIR := ./src/client
CLIENT_OBJ_DIR := ./obj/client
IMAGE_SRC_DIR := ./src/image
BIN_DIR     = ./bin

# Subdirectories for source files (can add more here)
CLIENT_SUBDIRS = $(CLIENT_SRC_DIR) $(IMAGE_SRC_DIR)

SERVER_SUBDIRS = $(SERVER_SRC_DIR) $(IMAGE_SRC_DIR)

# Find all .cpp files in the src/ and its subdirectories
SERVER_SOURCES = $(foreach dir,$(SERVER_SUBDIRS),$(wildcard $(dir)/*.cpp))

CLIENT_SOURCES = $(foreach dir,$(CLIENT_SUBDIRS),$(wildcard $(dir)/*.cpp))

# Create corresponding object files in obj/ directory, preserving subdirectory structure
SERVER_OBJECTS = $(patsubst $(SERVER_SRC_DIR)/%.cpp, $(SERVER_OBJ_DIR)/%.o, $(SERVER_SOURCES))

CLIENT_OBJECTS = $(patsubst $(CLIENT_SRC_DIR)/%.cpp, $(CLIENT_OBJ_DIR)/%.o, $(CLIENT_SOURCES))

SERVER_TARGET = $(BINDIR)/server
CLIENT_TARGET = $(BINDIR)/client

all: $(SERVER_TARGET) $(CLIENT_TARGET)

# Link object files to create the executable
$(SERVER_TARGET): $(SERVER_OBJECTS)
	@mkdir -p $(BINDIR)
	$(CXX) $(SERVER_OBJECTS) -o $(SERVER_TARGET)

$(CLIENT_TARGET): $(CLIENT_OBJECTS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CLIENT_OBJECTS) -o $(CLIENT_TARGET)

# Compile library object files
$(OBJDIR)/image/%.o: $(IMAGE_SRC_DIR)/%.cpp
	@mkdir -p $(OBJDIR)/image
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile .cpp files into .o object files
$(SERVER_OBJ_DIR)/%.o: $(SERVER_SRC_DIR)/%.cpp
	@mkdir -p $(SERVER_OBJ_DIR)  # Create the necessary directories for object files
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(CLIENT_OBJ_DIR)/%.o: $(CLIENT_SRC_DIR)/%.cpp
	@mkdir -p $(CLIENT_OBJ_DIR)  # Create the necessary directories for object files
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -rf $(OBJDIR) $(BINDIR)




