# # # # # # # # # # # # # # # #
#  Mackenzie Malainey, 1570494
#  Michael Kwok, INSERT ID
#  Assignment 2 Part 1
#  CMPUT 275, Winter 2020
# # # # # # # # # # # # # # # #

CC = g++

server: server.o dijkstra.o digraph.o
	$(CC) server.o digraph.o dijkstra.o -o server

server.o:
	$(CC) -c server.cpp

digraph.o:
	$(CC) -c digraph.cpp

dijkstra.o:
	$(CC) -c dijkstra.cpp

clean:
	rm -f digraph.o graph_concepts.o graph_concepts