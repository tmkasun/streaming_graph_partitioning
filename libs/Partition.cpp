#include "Partition.h"
#include <iostream>
#include <sstream>
#include <vector>

// This addition is undirectional , Add both items of the pair as keys
void Partition::addEdge(std::pair<int, int> edge) {
    auto exsistFirstVertext = this->edgeList.find(edge.first);
    if (exsistFirstVertext != this->edgeList.end()) {
        this->edgeList[edge.first].insert(edge.second);
    } else {
        this->edgeList[edge.first] = std::set<int>({edge.second});
    }

    auto exsistSecondVertext = this->edgeList.find(edge.second);
    if (exsistSecondVertext != this->edgeList.end()) {
        this->edgeList[edge.second].insert(edge.first);
    } else {
        this->edgeList[edge.second] = std::set<int>({edge.first});
    }
}

std::set<int> Partition::getNeighbors(int vertex) {
    auto exsist = this->edgeList.find(vertex);
    if (exsist != this->edgeList.end()) {
        return this->edgeList[vertex];
    } else {
        return {};
    }
}

// The number of edges, the cardinality of E, is called the size of graph and denoted by |E|. We usually use m to denote
// the size of G.
double Partition::edgesCount() {
    double total = 0;
    std::set<int> uniqueEdges;
    for (auto edge : this->edgeList) {
        for (auto vertext : edge.second) {
            uniqueEdges.insert(edge.first + vertext);
        }
    }
    return uniqueEdges.size();
}

// The number of vertices, the cardinality of V, is called the order of graph and devoted by |V|. We usually use n to
// denote the order of G.
double Partition::vertextCount() { return this->edgeList.size(); }

double Partition::partitionScore(int vertex) {
    double fillRatio = vertextCount() / *(this->maxSize);
    double neighbors = this->getNeighbors(vertex).size();
    return neighbors - fillRatio;
}

template <typename Out>
void Partition::_split(const std::string &s, char delim, Out result) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> Partition::_split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    Partition::_split(s, delim, std::back_inserter(elems));
    return elems;
}

/**
 * Expect a space seperated pair of vertexts representing an edge in the graph.
 **/
std::pair<int, int> Partition::deserialize(std::string data) {
    std::vector<std::string> v = Partition::_split(data, ' ');
    std::cout << "Vertext 1 = " << stoi(v[0]) << std::endl;
    std::cout << "Vertext 2 = " << stoi(v[1]) << std::endl;
    return {stoi(v[0]), stoi(v[1])};
}

void Partition::addToEdgeCuts(long resident, long foreign, int partitionId) {
    if (partitionId < this->edgeCuts.size()) {
        auto exsistResidentVertiext = this->edgeCuts[partitionId].find(resident);
        if (exsistResidentVertiext != this->edgeCuts[partitionId].end()) {
            this->edgeCuts[partitionId][resident].insert(foreign);
        } else {
            this->edgeCuts[partitionId][resident] = std::unordered_set<int>(foreign);
        }
    }
}

long Partition::edgeCutsCount() {
    long total = 0;
    for (auto partition : this->edgeCuts) {
        for (auto edgeCuts : partition) {
            total += edgeCuts.second.size();
        }
    }
    return total;
}

float Partition::edgeCutsRatio() { return this->edgesCount() / this->edgeCutsCount(); }