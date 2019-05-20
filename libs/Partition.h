#include <map>
#include <set>
#include <vector>

#ifndef JASMINE_PARTITION
#define JASMINE_PARTITION

class Partition {
    /*
    What is set good for? (Source: http://lafstern.org/matt/col1.pdf) 
        - The collection can potentially grow so large that the difference between O(N) and O(log N) is important.
        - The number of lookups is the same order of magnitude as the number of insertions; there aren't so few
            insertions that insertion speed is irrelevant.
        - Elements are inserted in random order, rather than being inserted in order.
        - Insertions and lookups are interleaved; we don't have distinct insertion and lookup phases.
    */
    std::map<int,std::set<int>> edgeList;
    int maxSize;

   public:
    Partition(int size) {this->maxSize = size;};
    void addEdge(std::pair<int, int> edge);
    std::set<int> getNeighbors(int);
    double partitionScore(int vertex);
    double edgesCount();
    double vertextCount();
};

#endif