#include <map>
#include <vector>

#ifndef JASMINE_PARTITION
#define JASMINE_PARTITION

class Partition {
    std::map<int,std::vector<int>> edgeList;
    int size;

   public:
    Partition(int size) {this->size = size;};
    void addEdge(std::pair<int, int> edge);
    std::vector<int> getNeighbors(int);
    double partitionScore(int vertex);
};

#endif