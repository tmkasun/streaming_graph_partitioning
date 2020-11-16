/**
Copyright 2020 JasminGraph Team
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
**/

#include "NodeManager.h"

#include <sys/stat.h>

#include "../../util/logger/Logger.h"
#include "NodeBlock.h"  // To setup node DB
#include "PropertyLink.h"
#include "RelationBlock.h"

Logger node_manager_logger;

NodeManager::NodeManager(std::string mode) {
    std::ios_base::openmode openMode = std::ios::trunc;  // default is Trunc mode which overrides the entire file
    if (mode == "app") {
        openMode = std::ios::app;  // if app, open in append mode
        this->nodeIndex = readNodeIndex();
    }
    NodeBlock::nodesDB =
        new std::fstream(NodeManager::NODE_DB_PATH, std::ios::in | std::ios::out | openMode | std::ios::binary);
    PropertyLink::propertiesDB =
        new std::fstream(PropertyLink::DB_PATH, std::ios::in | std::ios::out | openMode | std::ios::binary);
    RelationBlock::relationsDB =
        new std::fstream(RelationBlock::DB_PATH, std::ios::in | std::ios::out | openMode | std::ios::binary);
    // TODO (tmkasun): set PropertyLink nextPropertyIndex after validating by modulus check from file number of bytes

    if (dbSize(NodeManager::NODE_DB_PATH) % NodeBlock::BLOCK_SIZE != 0) {
        node_manager_logger.error("Node DB size does not complies to node block size Path = " +
                                  NodeManager::NODE_DB_PATH);
    }
}

std::unordered_map<std::string, unsigned int> NodeManager::readNodeIndex() {
    std::ifstream index_db(this->index_db_loc, std::ios::app | std::ios::binary);
    std::unordered_map<std::string, unsigned int> _nodeIndex;  // temproy node index data holder

    if (index_db.is_open()) {
        int iSize = dbSize(this->index_db_loc);
        unsigned long dataWidth = NodeManager::INDEX_KEY_SIZE + sizeof(unsigned int);
        if (iSize % dataWidth != 0) {
            node_manager_logger.error("Index DB size does not complies to index block size Path = " +
                                      this->index_db_loc);
            throw std::runtime_error("Node index DB in " + this->index_db_loc + " is corrupted!");
        }

        char nodeIDC[NodeManager::INDEX_KEY_SIZE];
        unsigned int nodeIndexId;
        for (size_t i = 0; i < iSize / dataWidth; i++) {
            nodeIDC[NodeManager::INDEX_KEY_SIZE] = {0};  // Fill with null chars before puting data
            if (!index_db.read(&nodeIDC[0], NodeManager::INDEX_KEY_SIZE)) {
                node_manager_logger.error("Error while reading index key data from block i = " + std::to_string(i));
            }
            if (!index_db.read(reinterpret_cast<char *>(&nodeIndexId), sizeof(unsigned int))) {
                node_manager_logger.error("Error while reading index ID data from block i = " + std::to_string(i));
            }
            _nodeIndex[std::string(nodeIDC)] = nodeIndexId;
        }
    } else {
        node_manager_logger.error("Error while opening the node index DB");
    }

    index_db.close();
    return _nodeIndex;
}

unsigned int NodeManager::addRelation(NodeBlock source, NodeBlock destination) {
    RelationBlock *newRelation = NULL;
    if (source.edgeRef == 0 || destination.edgeRef == 0 ||
        !source.searchRelation(destination)) {  // certainly a new relation block needed
        newRelation = RelationBlock::add(source, destination);
        if (newRelation) {
            source.updateRelation(newRelation);
            destination.updateRelation(newRelation);
        } else {
            node_manager_logger.error("Something went wrong while adding the new edge/relation source = " +
                                      std::string(source.id) + " destination = " + std::string(destination.id));
        }

    } else {
        node_manager_logger.warn("Relation/Edge already exist for source = " + std::string(source.id) +
                                 " destination = " + std::string(destination.id));
    }
    return 0;
}

NodeBlock *NodeManager::addNode(std::string nodeId) {
    unsigned int assignedNodeIndex;
    if (this->nodeIndex.find(nodeId) == this->nodeIndex.end()) {
        node_manager_logger.debug("NodeId not found in index for node ID " + nodeId);
        NodeBlock *sourceBlk = new NodeBlock(nodeId, this->nextNodeIndex * NodeBlock::BLOCK_SIZE);
        this->nodeIndex.insert({nodeId, this->nextNodeIndex});
        assignedNodeIndex = this->nextNodeIndex;
        this->nextNodeIndex++;
        sourceBlk->save();
        return sourceBlk;
    } else {
        node_manager_logger.debug("NodeId found in index for node ID " + nodeId);
        return this->get(nodeId);
    }
}

std::pair<NodeBlock, NodeBlock> NodeManager::addEdge(std::pair<std::string, std::string> edge) {
    NodeBlock *sourceNode = this->addNode(edge.first);
    NodeBlock *destNode = this->addNode(edge.second);
    unsigned int relationAddr = this->addRelation(*sourceNode, *destNode);

    node_manager_logger.debug("DEBUG: Source DB block address " + std::to_string(sourceNode->addr) +
                              " Destination DB block address " + std::to_string(destNode->addr));
    std::pair<NodeBlock, NodeBlock> returnVal = {*sourceNode, *destNode};
    delete sourceNode;
    delete destNode;
    return returnVal;
}

int NodeManager::dbSize(std::string path) {
    /*
        The structure stat contains at least the following members:
        st_dev     ID of device containing file
        st_ino     file serial number
        st_mode    mode of file (see below)
        st_nlink   number of links to the file
        st_uid     user ID of file
        st_gid     group ID of file
        st_rdev    device ID (if file is character or block special)
        st_size    file size in bytes (if file is a regular file)
        st_atime   time of last access
        st_mtime   time of last data modification
        st_ctime   time of last status change
        st_blksize a filesystem-specific preferred I/O block size for
                        this object.  In some filesystem types, this may
                        vary from file to file
        st_blocks  number of blocks allocated for this object
    */
    struct stat result;
    if (stat(path.c_str(), &result) == 0) {
        node_manager_logger.debug("Size of the " + path + " is " + std::to_string(result.st_size));
    } else {
        node_manager_logger.error("Error while reading file stats in " + path);
        return -1;
    }

    return result.st_size;
}

/**
 * @Deprecated use NodeBlock.get() instead
 **/
NodeBlock *NodeManager::get(std::string nodeId) {
    NodeBlock *nodeBlockPointer = NULL;
    int debug = std::stoi(nodeId);
    if (this->nodeIndex.find(nodeId) == this->nodeIndex.end()) {  // Not found
        return nodeBlockPointer;
    }
    unsigned int nodeIndex = this->nodeIndex[nodeId];
    const unsigned int blockAddress = nodeIndex * NodeBlock::BLOCK_SIZE;
    NodeBlock::nodesDB->seekg(blockAddress);
    unsigned int edgeRef;
    unsigned int propRef;
    char usageBlock;
    char label[6];

    if (!NodeBlock::nodesDB->get(usageBlock)) {
        node_manager_logger.error("Error while reading usage data from block " + std::to_string(blockAddress));
    }

    if (!NodeBlock::nodesDB->read(reinterpret_cast<char *>(&edgeRef), sizeof(unsigned int))) {
        node_manager_logger.error("Error while reading edge reference data from block " + std::to_string(blockAddress));
    }

    if (!NodeBlock::nodesDB->read(reinterpret_cast<char *>(&propRef), sizeof(unsigned int))) {
        node_manager_logger.error("Error while reading prop reference data from block " + std::to_string(blockAddress));
    }

    if (!NodeBlock::nodesDB->read(&label[0], 6)) {
        node_manager_logger.error("Error while reading label data from block " + std::to_string(blockAddress));
    }
    bool usage = usageBlock == '\1';
    node_manager_logger.debug("Label = " + std::string(label));
    node_manager_logger.debug("Length of label = " + std::to_string(strlen(label)));
    node_manager_logger.debug("DEBUG: raw edgeRef from DB (disk) " + std::to_string(edgeRef));

    nodeBlockPointer = new NodeBlock(nodeId, blockAddress, propRef, edgeRef, label, usage);

    node_manager_logger.debug("DEBUG: nodeBlockPointer after creating the object edgeRef" +
                              std::to_string(nodeBlockPointer->edgeRef));

    if (nodeBlockPointer->edgeRef % RelationBlock::BLOCK_SIZE != 0) {
        throw "Exception: Invalid edge reference address = " + nodeBlockPointer->edgeRef;
    }
    return nodeBlockPointer;
}

void NodeManager::persistNodeIndex() {
    std::ofstream index_db(this->index_db_loc, std::ios::trunc | std::ios::binary);
    if (index_db.is_open()) {
        for (auto nodeMap : this->nodeIndex) {
            char nodeIDC[NodeManager::INDEX_KEY_SIZE] = {0};  // Initialize with null chars
            std::strcpy(nodeIDC, nodeMap.first.c_str());
            index_db.write(nodeIDC, sizeof(nodeIDC));
            unsigned int nodeBlockIndex = nodeMap.second;
            index_db.write(reinterpret_cast<char *>(&(nodeBlockIndex)), sizeof(unsigned int));
            node_manager_logger.debug("Writing node index --> Node key = " + std::string(nodeIDC) + " value " +
                                      std::to_string(nodeBlockIndex));
        }
    }
    index_db.close();
}

/**
 * Return the number of nodes upto the limit given in the arg from nodes index
 * Default limit is 10
 * */
std::list<NodeBlock> NodeManager::getGraph(int limit) {
    int i = 0;
    std::list<NodeBlock> vertices;
    for (auto it : this->nodeIndex) {
        i++;
        if (i >= limit) {
            break;
        }
        auto nodeId = it.first;
        NodeBlock *node = this->get(nodeId);
        vertices.push_back(*node);
    }
    return vertices;
}

void NodeManager::close() {
    this->persistNodeIndex();
    if (PropertyLink::propertiesDB) {
        PropertyLink::propertiesDB->flush();
        PropertyLink::propertiesDB->close();
    }
    if (NodeBlock::nodesDB) {
        NodeBlock::nodesDB->flush();
        NodeBlock::nodesDB->close();
    }
}

const unsigned long NodeManager::INDEX_KEY_SIZE = 8;
unsigned int NodeManager::nextPropertyIndex = 0;
std::string NodeManager::NODE_DB_PATH = "streamStore/nodes.db";