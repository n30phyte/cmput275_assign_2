# # # # # # # # # # # # # # # #
#  Mackenzie Malainey, 1570494
#  Michael Kwok, 1548454
#  Assignment 2 Part 1
#  CMPUT 275, Winter 2020
# # # # # # # # # # # # # # # #

CC = g++
CFLAGS = -Wall

# Directory for desktop build files
BUILD_DIR = build

server: server.o dijkstra.o digraph.o serialport.o
	cd $(BUILD_DIR); \
	$(CC) server.o digraph.o dijkstra.o serialport.o -o ../server

server.o: server.cpp $(BUILD_DIR)
	$(CC) -c server.cpp -o $(BUILD_DIR)/server.o $(CFLAGS)

digraph.o: digraph.cpp digraph.h $(BUILD_DIR)
	$(CC) -c digraph.cpp -o $(BUILD_DIR)/digraph.o $(CFLAGS)

dijkstra.o: dijkstra.cpp dijkstra.h $(BUILD_DIR)
	$(CC) -c dijkstra.cpp -o $(BUILD_DIR)/dijkstra.o $(CFLAGS)

serialport.o: serialport.cpp serialport.h $(BUILD_DIR)
	$(CC) -c serialport.cpp -o $(BUILD_DIR)/serialport.o $(CFLAGS)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

clean:
	rm -f server $(BUILD_DIR) -r