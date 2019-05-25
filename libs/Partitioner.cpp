#include <iostream>
#include <vector>
#include <algorithm>

#include "Partitioner.h"


void Partitioner::addEdge(std::pair<long, long> edge) {
    this->totalVertices += 2;
    this->perPartitionCap = totalVertices / numberOfPartitions;

    switch (this->algorithmInUse) {
        case Algorithms::HASH:
            this->hashPartitioning(edge);
            break;
        case Algorithms::FENNEL:
            this->fennelPartitioning(edge);
            break;
        // case Algorithms::LDG:
        //     this->ldgPartitioning(edge);
        //     break;
        default:
            break;
    }
}

void Partitioner::hashPartitioning(std::pair<int, int> edge) {
    int firstIndex = edge.first % this->numberOfPartitions;    // Hash partitioning
    int secondIndex = edge.second % this->numberOfPartitions;  // Hash partitioning

    if (firstIndex == secondIndex) {
        this->partitions[firstIndex].addEdge(edge);
    } else {
        this->partitions[firstIndex].addToEdgeCuts(edge.first, edge.second, secondIndex);
        this->partitions[secondIndex].addToEdgeCuts(edge.second, edge.first, firstIndex);
    }
}

void Partitioner::printStats() {
    int id = 0;
    for (auto partition : this->partitions) {
        std::cout << id << " => Vertext count = " << partition.vertextCount() << std::endl;
        std::cout << id << " => Edges count = " << partition.edgesCount() << std::endl;
        std::cout << id << " => Edge cuts count = " << partition.edgeCutsCount() << std::endl;
        std::cout << id << " => Cut ratio = " << partition.edgeCutsRatio() << std::endl;
        partition.printEdgeCuts();
        partition.printEdges();
        id++;
    }
}

void Partitioner::fennelPartitioning(std::pair<int, int> edge) {
    
    std::vector<long> partitionScoresFirst(numberOfPartitions, 0);  // Calculate per incoming edge
    std::vector<long> partitionScoresSecond(numberOfPartitions, 0);                    // Calculate per incoming edge

    int id = 0;
    for (auto partition : partitions) {
        long currentSize = partition.vertextCount();
        long thisCostSecond, thisCostFirst = 0;

        long ngFirst = partition.getNeighbors(edge.first).size();
        if (perPartitionCap != 0 && false) {
            thisCostFirst = ngFirst - currentSize / (perPartitionCap);
        } else {
            thisCostFirst = ngFirst;
        }

        if (thisCostFirst == 0 && perPartitionCap != 0) {
            thisCostFirst = -currentSize / (perPartitionCap);
        }

        partitionScoresFirst[id] = thisCostFirst;

        long ngSecond = partition.getNeighbors(edge.first).size();
        if (perPartitionCap != 0 && false) {
            thisCostSecond = ngSecond - currentSize / (perPartitionCap);
        } else {
            thisCostSecond = ngSecond;
        }
        if (thisCostSecond == 0 && perPartitionCap != 0) {
            thisCostSecond = -currentSize / (perPartitionCap);
        }
        partitionScoresSecond[id] = thisCostSecond;

        id++;
    }

    int firstIndex =
        distance(partitionScoresFirst.begin(), max_element(partitionScoresFirst.begin(), partitionScoresFirst.end()));
    int secondIndex = distance(partitionScoresSecond.begin(),
                               max_element(partitionScoresSecond.begin(), partitionScoresSecond.end()));
    if (firstIndex == secondIndex) {
        partitions[firstIndex].addEdge(edge);
    } else {
        partitions[firstIndex].addToEdgeCuts(edge.first, edge.second, secondIndex);
        partitions[secondIndex].addToEdgeCuts(edge.second, edge.first, firstIndex);
    }
}
