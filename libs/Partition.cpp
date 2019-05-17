#include "Partition.h"
#include <iostream>
#include <vector>

void Partition::addEdge(std::pair<int, int> edge) {
    auto exsist = this->edgeList.find(edge.first);
    if (exsist != this->edgeList.end()) {
        this->edgeList[edge.first].push_back(edge.second);
    } else {
        this->edgeList.insert(edge);
    }
}

std::vector<int> Partition::getNeighbors(int vertex) {
    auto exsist = this->edgeList.find(vertex);
    if (exsist != this->edgeList.end()) {
        return this->edgeList[vertex];
    } else {
        return {};
    }
}

double Partition::partitionScore(int vertex) {
    double fillRatio = this->edgeList.count / this->size;
    double neighbors = this->getNeighbors(vertex).size;
    return neighbors - fillRatio;
}