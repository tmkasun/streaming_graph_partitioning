#include "Partition.h"
#include <iostream>
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
    double fillRatio = vertextCount() / this->maxSize;
    double neighbors = this->getNeighbors(vertex).size();
    return neighbors - fillRatio;
}