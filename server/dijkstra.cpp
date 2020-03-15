/**
 * Mackenzie Malainey, 1570494
 * Michael Kwok, 
 * Assign #2 Part 1
 * CMPUT 275, Winter 2020
 */

#include "dijkstra.h"
#include "heap.h"

/**
 *
 * Description:
 * Implementation of the Dijkstra weighted graph search algorithm
 *
 * Parameters:
 * graph (const WDigraph&): graph to build search tree from
 * startVertex (int): starting node
 * tree (unordered_map<int, PIL>&): Search tree to populate
 *
 */
void dijkstra(const WDigraph& graph, int startVertex,
              unordered_map<int, PIL>& tree) {

    BinaryHeap<pair<int, int>, long long> events;

    events.insert(pair<int, int>(startVertex, startVertex), 0);

    // Loop through graph adding events
    while (events.size() > 0) {
        HeapItem<pair<int, int>, long long> item = events.min();
        events.popMin();

        int v = item.item.second;

        if (tree.count(v) == 0) {
            tree[v] = PIL(item.item.first, item.key);

            // Add neightbours
            for(auto it = graph.neighbours(v); it != graph.endIterator(v); it++) {
                if (tree.count(*it) == 0) // Don't bother with already touched nodes
                    events.insert(pair<int, int>(v, *it), item.key + graph.getCost(v, *it));
            }
        }
    }
}
