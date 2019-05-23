#include <map>
#include <set>
#include <unordered_set>
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
    std::map<int, std::set<int>> edgeList;
    std::vector<std::map<long, std::unordered_set<int>>> edgeCuts;
    int id;
    int numberOfPartitions;  // Size of the cluster
    int *maxSize;            // Current maximum number of vertices that can be stored in this partition

   public:
    Partition(int id, int numberOfPartitions) {
        this->id = id;
        this->numberOfPartitions = numberOfPartitions;
        for (size_t i = 0; i < numberOfPartitions; i++) {
            this->edgeCuts.push_back({});
        }
    };
    void addEdge(std::pair<int, int> edge);
    std::set<int> getNeighbors(int);
    double partitionScore(int vertex);
    double edgesCount();
    double vertextCount();
    void addToEdgeCuts(long resident, long foriegn, int partitionId);
    // maxSize is : Number of vertices that can be store in this partition, This is a dynamic shared pointer containing
    // a value depending on the whole graph size and # of partitions
    void setMaxSize(int *size) { this->maxSize = size; };
    int getMaxSize() { return *(this->maxSize); };
    float edgeCutsRatio();
    template <typename Out>
    static void _split(const std::string &s, char delim, Out result);
    static std::vector<std::string> _split(const std::string &s, char delim);
    static std::pair<int, int> deserialize(std::string data);
    long edgeCutsCount();
};

#endif