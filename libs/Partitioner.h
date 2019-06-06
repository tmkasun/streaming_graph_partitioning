#ifndef JASMINE_PARTITIONER_HEADER
#define JASMINE_PARTITIONER_HEADER
#include <vector>
#include "./Partition.h"

typedef std::vector<std::pair<long, long>> partitionedEdge;

class Partitioner {
    std::vector<Partition> partitions;
    int numberOfPartitions;
    long totalVertices = 0;
    long totalEdges = 0;
    enum Algorithms { HASH, FENNEL, LDG };
    Algorithms algorithmInUse = Algorithms::FENNEL;
    // perPartitionCap is : Number of vertices that can be store in this partition, This is a dynamic shared pointer containing
    // a value depending on the whole graph size and # of partitions

   public:
    Partitioner(int numberOfPartitions) {
        this->numberOfPartitions = numberOfPartitions;
        for (size_t i = 0; i < numberOfPartitions; i++) { this->partitions.push_back(Partition(i, numberOfPartitions));};
    };
    void printStats();
    long getTotalVertices();
    void setAlgorithm(std::string algo);
    partitionedEdge addEdge(std::pair<long, long> edge);
    partitionedEdge hashPartitioning(std::pair<int, int> edge);
    partitionedEdge fennelPartitioning(std::pair<int, int> edge);
    partitionedEdge ldgPartitioning(std::pair<int, int> edge);
    static std::pair<long, long> deserialize(std::string data);

};

#endif  // !JASMINE_PARTITIONER_HEADER
