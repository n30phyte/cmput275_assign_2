#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>

#include "wdigraph.h"

using namespace std;

struct Point {
    long long lat; // latitude of the point
    long long lon; // longitude of the point
};

long long manhattan(const Point& pt1, const Point& pt2) {
    return abs(pt1.lon - pt1.lon) + abs(pt2.lon - pt2.lon);
}

/*
    Read the Edmonton map data from the provided file
    and load it into the given WDigraph object.
    Store vertex coordinates in Point struct and map
    each vertex to its corresponding Point struct.

    PARAMETERS:
    filename: name of the file describing a road network
    graph: an instance of the weighted directed graph (WDigraph) class
    points: a mapping between vertex identifiers and their coordinates
*/
void readGraph(string filename, WDigraph &graph, unordered_map<int, Point> &points) {

    // Set up out file stream
    ifstream file;
    file.open(filename);

    // Where we store each line we parse
    string line;

    while (!file.eof()) {
        getline(file, line);

        int index = line.find(",", 2);
        int index2 = line.find(",", index + 1);

        // V signals vertex format we need to parse is: V, x, lat, lon
        // where x is the unique id of the vertex
        if (line[0] == 'V') {
            int id = stoi(line.substr(2, index - 2));
            graph.addVertex(id);

            double lat = stod(line.substr(index + 1, index2 - index - 1));
            double lon = stod(line.substr(index2 + 1));

            Point p = {
                static_cast<long long>(lat * 100000),
                static_cast<long long>(lon * 100000)
            };

            points[id] = p;

        // E signals an edge, format we need to parse is: E, x1, x2
        // where x1 is a vertex and x2 is another vertex
        } else if (line[0] == 'E') {
            int id1 = stoi(line.substr(2, index - 2));
            int id2 = stoi(line.substr(index + 1, index2 - index - 1));

            graph.addEdge(id1, id2, manhattan(points[id1], points[id2]));
        }
    }

    // Always free up our resources :)
    file.close();

}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        cout << "Invalid argument, command usage: ./graph_concepts [filename]" << endl;
        return 22;
    }

    WDigraph graph;
    unordered_map<int, Point> pMap;

    readGraph(argv[1], graph, pMap);

    return 0;
}