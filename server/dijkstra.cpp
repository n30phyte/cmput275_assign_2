#include "dijkstra.h"
#include "heap.h"

void dijkstra(const WDigraph& graph, int startVertex,
              unordered_map<int, PIL>& tree) {

    BinaryHeap<pair<int, int>, long long> events;

    events.insert(pair<int, int>(startVertex, startVertex), 0);

    while (events.size() > 0) {
        HeapItem<pair<int, int>, long long> item = events.min();
        events.popMin();

        int v = item.item.second;

        if (tree.count(v) == 0) {
            tree[v] = PIL(item.item.first, item.key);
            unordered_set<int>::const_iterator n = graph.neighbours(v);
            while (n != graph.endIterator(v)) {
                events.insert(pair<int, int>(v, *n), item.key + graph.getCost(v, *n));
                n++;
            }
        }
    }
}