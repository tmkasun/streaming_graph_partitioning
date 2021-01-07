#ifndef JASMINE_PARTITIONER_HEADER
#define JASMINE_PARTITIONER_HEADER
#include <vector>

#include "../libs/graphStore/NodeManager.h"
#include "./Partition.h"

typedef std::vector<std::pair<std::string, long>> partitionedEdge;
namespace spt { // spt : Streaming Partitioner
enum Algorithms { HASH, FENNEL, LDG };

}

class Partitioner {
    std::vector<Partition> partitions;
    int numberOfPartitions;
    long totalVertices = 0;
    long totalEdges = 0;
    int graphID;
    spt::Algorithms algorithmInUse;
    // perPartitionCap is : Number of vertices that can be store in this partition, This is a dynamic shared pointer
    // containing a value depending on the whole graph size and # of partitions

   public:
    Partitioner(int numberOfPartitions, int graphID, spt::Algorithms alog)
        : numberOfPartitions(numberOfPartitions), graphID(graphID), algorithmInUse(alog) {
        for (size_t i = 0; i < numberOfPartitions; i++) {
            this->partitions.push_back(Partition(i, numberOfPartitions));
        };
    };
    void printStats();
    long getTotalVertices();
    void setAlgorithm(std::string algo);
    partitionedEdge addEdge(std::pair<std::string, std::string> edge);
    partitionedEdge hashPartitioning(std::pair<std::string, std::string> edge);
    partitionedEdge fennelPartitioning(std::pair<std::string, std::string> edge);
    partitionedEdge ldgPartitioning(std::pair<std::string, std::string> edge);
    static std::pair<long, long> deserialize(std::string data);
};

#endif  // !JASMINE_PARTITIONER_HEADER
