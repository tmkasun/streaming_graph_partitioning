#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

#include "Partitioner.h"

void Partitioner::addEdge(std::pair<long, long> edge) {
    this->totalVertices += 2;

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

/**
 * Greedy vertex assignment objectives of minimizing the number of cut edges
and balancing of the partition sizes. Assign the vertext to partition P that maximize the partition score
 * |N(v) ∩ Si| + ∂c(|Si|)
 * |N(v) ∩ Si| =  number of neighbours of vertex v that are assigned to partition Si.
 * ∂c(|Si|) = the marginal cost of increasing the partition i by one additional vertex.
 *
 * Intra-partition cost function : c(x) = αx^γ, for α > 0 and γ ≥ 1 where x = vertex cardinality
 *   α = m*(k^γ-1) / (n^γ)
 *   Total number of vertices and edges in the graph denoted as |V| = n and |E| = m.
 *   k is number of partitions
 **/
void Partitioner::fennelPartitioning(std::pair<int, int> edge) {
    std::vector<long> partitionScoresFirst(numberOfPartitions, 0);   // Calculate per incoming edge
    std::vector<long> partitionScoresSecond(numberOfPartitions, 0);  // Calculate per incoming edge
    const double gamma = 3 / 2.0;
    const double alpha =
        this->totalEdges * pow(this->numberOfPartitions, (gamma - 1)) / pow(this->totalVertices, gamma);
    bool firstVertextAlreadyExist(false);
    bool secondVertextAlreadyExist(false);

    int id = 0;
    for (auto partition : partitions) {
        long currentSize = partition.vertextCount();
        long thisCostSecond, thisCostFirst = 0;
        std::set<int> firstVertextNeighbors = partition.getNeighbors(edge.first);
        std::set<int> secondVertextNeighbors = partition.getNeighbors(edge.first);
        double firstVertextIntraCost;
        double secondVertextIntraCost;

        double firstVertextInterCost = firstVertextNeighbors.size();
        double secondVertextInterCost = secondVertextNeighbors.size();

        if (firstVertextNeighbors.size() == 0) {
            firstVertextIntraCost = alpha * gamma * pow(firstVertextNeighbors.size(), (gamma - 1));
        } else {
            if (firstVertextNeighbors.find(edge.second) != firstVertextNeighbors.end())
                return;  // Nothing to do, edge already exisit
            firstVertextAlreadyExist = true;
            firstVertextIntraCost = 0;  // No cost of adding the vertext to this partition because it's already exist
        }

        partitionScoresFirst[id] = firstVertextInterCost - firstVertextIntraCost;

        if (secondVertextNeighbors.size() == 0) {
            secondVertextIntraCost = alpha * gamma * pow(secondVertextNeighbors.size(), (gamma - 1));
        } else {
            // edge can't already exists at this pont so no need to check if(secondVertextNeighbors.find(edge.second) !=
            // secondVertextNeighbors.end()) return; // Nothing to do, edge already exisit
            secondVertextAlreadyExist = true;
            secondVertextIntraCost = 0;  // No cost of adding the vertext this partition because it's already exist
        }

        partitionScoresSecond[id] = secondVertextInterCost - secondVertextIntraCost;

        // long ngFirst = partition.getNeighbors(edge.first).size();

        // if (perPartitionCap != 0) {
        //     thisCostFirst = ngFirst - currentSize / (perPartitionCap);
        // } else {
        //     thisCostFirst = ngFirst;
        // }

        // if (thisCostFirst == 0 && perPartitionCap != 0) {
        //     thisCostFirst = -currentSize / (perPartitionCap);
        // }

        // partitionScoresFirst[id] = thisCostFirst;

        // long ngSecond = partition.getNeighbors(edge.first).size();
        // if (perPartitionCap != 0) {
        //     thisCostSecond = ngSecond - currentSize / (perPartitionCap);
        // } else {
        //     thisCostSecond = ngSecond;
        // }
        // if (thisCostSecond == 0 && perPartitionCap != 0) {
        //     thisCostSecond = -currentSize / (perPartitionCap);
        // }
        // partitionScoresSecond[id] = thisCostSecond;

        id++;
    }
    if (!firstVertextAlreadyExist) this->totalVertices += 1;
    if (!secondVertextAlreadyExist) this->totalVertices += 1;

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

/**
 * Expect a space seperated pair of vertexts representing an edge in the graph.
 **/
std::pair<long, long> Partitioner::deserialize(std::string data) {
    std::vector<std::string> v = Partition::_split(data, ' ');
    std::cout << "Vertext 1 = " << stoi(v[0]) << std::endl;
    std::cout << "Vertext 2 = " << stoi(v[1]) << std::endl;
    return {stoi(v[0]), stoi(v[1])};
}
