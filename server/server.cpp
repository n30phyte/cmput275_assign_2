/**
 * Mackenzie Malainey, 1570494
 * Michael Kwok, 1548454
 * Assign #2 Part 1
 * CMPUT 275, Winter 2020
 */

#include <fstream>
#include <iostream>
#include <stack>
#include <string>

#include "dijkstra.h"
#include "wdigraph.h"

const char MAP_FILE_NAME[] = "edmonton-roads-2.0.1.txt";

using namespace std;

struct Point {
  long long lat;  // latitude of the point
  long long lon;  // longitude of the point
};

/**
 * Calculates manhattan distance between two points.
 *
 * @param pt1: First point
 * @param pt2: Second point
 */
long long manhattan(const Point &pt1, const Point &pt2) {
  return abs(pt1.lat - pt2.lat) + abs(pt1.lon - pt2.lon);
}

/**
 * Reads the Edmonton map data from the file in the filename parameter
 * and saves it in to a WDigraph object specified in the graph parameter.
 *
 * Each vertex coordinate is stored in a Point object and gets corellated in
 * an unordered_map
 *
 * @param filename: name of a CSV file describing a road network
 * @param graph: an instance of the WDigraph class
 * @param points: a mapping between vertex identifiers and their coordinates
 */
void readGraph(const string &filename, WDigraph &graph,
               unordered_map<int, Point> &points) {
  // Set up out file stream
  ifstream file;
  file.open(filename);

  // Where we store each line we parse
  string line;

  while (!file.eof()) {
    getline(file, line);

    int index = line.find(',', 2);
    int index2 = line.find(',', index + 1);

    // V signals vertex format we need to parse is: V, x, lat, lon
    // where x is the unique id of the vertex
    if (line[0] == 'V') {
      int id = stoi(line.substr(2, index - 2));
      graph.addVertex(id);

      double lat = stod(line.substr(index + 1, index2 - index - 1));
      double lon = stod(line.substr(index2 + 1));

      Point p = {static_cast<long long>(lat * 100000),
                 static_cast<long long>(lon * 100000)};

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

/* Paramters:
 *  (int):
 *
 *
 * Returns:
 * path (stack<Point>*):
 *
 */
/**
 * Given a start and end vertex return the shortest path to the vertex
 *
 * @param a: Start vertex
 * @param b: End vertex
 * @param graph: Weighted directed graph representing road map
 * @param map: Map that associates each vertice with a Point obj
 *
 * @return: Path from start to end vertex as a stack of Point objs,
 * returns an empty stack if no such path exists
 */
stack<Point> *getPath(int a, int b, WDigraph &graph,
                      unordered_map<int, Point> &map) {
  auto *route = new stack<Point>();

  unordered_map<int, PIL> tree;

  // Build search tree
  dijkstra(graph, a, tree);

  int vertex = b;

  // Build path from search tree, if path exists
  if (tree.count(vertex) > 0) {
    do {
      PIL next = tree[vertex];
      route->push(map[vertex]);
      vertex = next.first;
    } while (vertex != a);

    route->push(map[a]);
  }

  return route;
}

/**
 * Gets the closest vertex ids to the given points
 *
 * @param p1: First point to clip
 * @param p2: Second point to clip
 * @param map: Map that provides vertexes associated with known points
 *
 * @return: Vertex ID of the closest points to given points in respective order
 */
pair<int, int> getVertices(const Point p1, const Point p2,
                           unordered_map<int, Point> &map) {
  // Instead of running this function twice for each point we know it will only
  // ever get ran when requiring

  pair<int, int> vs(map.begin()->first, map.begin()->first);

  for (unordered_map<int, Point>::const_iterator iter = map.begin();
       iter != map.end(); iter++) {
    if (manhattan(iter->second, p1) < manhattan(map[vs.first], p1))
      vs.first = iter->first;
    if (manhattan(iter->second, p2) < manhattan(map[vs.second], p2))
      vs.second = iter->first;
  }

  return vs;
}

// Enum of states for the FSM used to control the server.
enum State { WaitingForRequest, Processing, PrintOutput, Done };

int main() {
  WDigraph graph;
  unordered_map<int, Point> pMap;

  readGraph(MAP_FILE_NAME, graph, pMap);

  State st = WaitingForRequest;

  Point start;
  Point end;

  stack<Point> *path = nullptr;

  string command;

  bool isDone = false;

  while (!isDone) {
    switch (st) {
      case WaitingForRequest:
        cin >> command;

        if (command == "R") {
          // Request a path
          cin >> start.lat >> start.lon >> end.lat >> end.lon;
          // Switch to process request
          st = Processing;
        }
        break;

      case Processing: {
        // Get pair of vertices for start and end.
        auto vertices = getVertices(start, end, pMap);

        // Generate paths
        path = getPath(vertices.first, vertices.second, graph, pMap);
      }  // Scoped to avoid having to declare a pair of points before loop
        st = PrintOutput;
        break;

      case PrintOutput:
        // Print size of path
        cout << "N " << path->size() << endl;

        // Only go through with printing waypoints when not empty.
        if (!path->empty()) {
          string ack;
          cin >> ack;

          while (!path->empty()) {
            if (ack == "A") {
              Point top = path->top();
              path->pop();
              cout << "W " << top.lat << ' ' << top.lon << endl;
            }
            cin >> ack;
          }

          cout << "E" << endl;
        }
        delete path;
        st = Done;
        break;

      case Done:
        isDone = true;
        break;
    }
  }

  return 0;
}
