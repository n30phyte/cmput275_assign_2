#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stack>
#include <string>

#include "wdigraph.h"
#include "dijkstra.h"

const char MAP_FILE_NAME[] = "edmonton-roads-2.0.1.text";

using namespace std;

struct Point {
    long long lat; // latitude of the point
    long long lon; // longitude of the point
};

long long manhattan(const Point& pt1, const Point& pt2) {
    return abs(pt1.lat - pt1.lat) + abs(pt1.lon - pt2.lon);
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

    cout << "Reading graph from " << filename << endl;

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

stack<Point>* getPath(int a, int b, WDigraph &graph, unordered_map<int, Point> &map) {

    stack<Point>* route = new stack<Point>();

    unordered_map<int, PIL> tree;

    dijkstra(graph, a, tree);

    int vertex = b;

    do {
        PIL next = tree[vertex];
        route->push(map[vertex]);
        vertex = next.first;
    } while (vertex != a);

    route->push(map[a]);

    return route;
}



pair<int, int> getVertices(const Point p1, const Point p2, unordered_map<int, Point> &map) {
    // Instead of running this function twice for each point we know it will only ever get ran
    // when requiring

    pair<int, int> vs(-1,-1);

    for (unordered_map<int, Point>::const_iterator iter = map.begin(); iter != map.end(); iter++) {
        if (manhattan(iter->second, p1) > manhattan(map[vs.first], p1)) vs.first = iter->first;
        if (manhattan(iter->second, p2) > manhattan(map[vs.second], p2)) vs.second = iter->first;
    }

    return vs;

}

void foo() {
    Point start;
    Point end;



}

int main() {

    WDigraph graph;
    unordered_map<int, Point> pMap;

    readGraph(MAP_FILE_NAME, graph, pMap);

    return 0;
}

