# Compiler
CXX = g++

# Compiler Flags
CXXFLAGS = -Wall -Wextra -std=c++17 -pthread

# Libraries
LIBS = -lreadline

# Targets
SERVER = server
CLIENT = client

all: $(SERVER) $(CLIENT)

$(SERVER): server.cpp
	$(CXX) $(CXXFLAGS) server.cpp -o $(SERVER)

$(CLIENT): client.cpp
	$(CXX) $(CXXFLAGS) client.cpp -o $(CLIENT) $(LIBS)

clean:
	rm -f $(SERVER) $(CLIENT)