#include <cppkafka/cppkafka.h>
#include <spdlog/spdlog.h>
#include <time.h>

#include <iostream>
#include <nlohmann/json.hpp>
#include <thread>  // FOr running websocket server parallelly

#include "libs/Partition.h"
#include "libs/Partitioner.h"
#include "libs/graphStore/NodeManager.h"
#include "libs/headers/KafkaTest.h"

#ifdef ENABLEWS
#include "libs/transports/websocket/WebSocketServer.h"
#endif  // ENABLEWS

using namespace std;
// for convenience
using json = nlohmann::json;

int main(int argc, char* argv[]) {
    spdlog::set_level(spdlog::level::debug);

    spdlog::info("Testing streaming store . . .");
    clock_t start = clock();
    Partitioner graphPartitioner(4);
    NodeManager* nm = new NodeManager("trunk");
#ifdef ENABLEWS
    BroadcastServer edgesWSServer;
    std::thread senderThread;
    uint16_t websocketPort = 9002;
    senderThread = std::thread(std::bind(&BroadcastServer::run, &edgesWSServer, websocketPort));
#endif  // ENABLEWS

    cppkafka::Configuration configs = {{"metadata.broker.list", "127.0.0.1:9092"}, {"group.id", "knnect"}};
    // cppkafka::Consumer cons(configs);
    KafkaTest ktesting(configs);

    ktesting.Subscribe("testtopic");

    while (true) {
        spdlog::debug("Waiting for a message . . .");
        cppkafka::Message msg = ktesting.consumer.poll();
        spdlog::debug("Polling message received");
        if (!msg) {
            spdlog::debug("Not a valid message");
            continue;
        }

        if (msg.get_error()) {
            spdlog::error("Error in received message");
            if (msg.is_eof()) {
                spdlog::info("Received a message with EOF tag");
            }
            continue;
        }

        spdlog::info("Received a valid message from topic = {}", msg.get_topic());
        spdlog::debug("Message received from partition ID = {}", msg.get_partition());
        spdlog::debug("Message offset = {}", msg.get_offset());
        string data(msg.get_payload());
        spdlog::debug("Message payload = {}", data);

        if (data == "-1") {  // Marks the end of stream
            spdlog::warn("Received end of graph stream signal (-1 )");
            break;
        }
        // parse explicitly
        auto edgeJson = json::parse(data);
        auto sourceJson = edgeJson["source"];
        auto destinationJson = edgeJson["destination"];
        std::string sId = std::string(sourceJson["id"]);
        std::string dId = std::string(destinationJson["id"]);

        std::pair<long, long> edge = {std::stol(sId), std::stol(dId)};
        partitionedEdge pe = graphPartitioner.addEdge(edge);

        pair<NodeBlock, NodeBlock> nodes = nm->addEdge({sId, dId});
        if (sourceJson.contains("properties")) {
            auto sourceProps = sourceJson["properties"];
            for (auto it = sourceProps.begin(); it != sourceProps.end(); it++) {
                spdlog::debug("Key = {} value = ", it.key(), it.value());
                string value = it.value().get<std::string>();
                nodes.first.addProperty(string(it.key()), &value[0]);
            }
        }
        if (destinationJson.contains("properties")) {
            auto destProps = destinationJson["properties"];
            for (auto it = destProps.begin(); it != destProps.end(); it++) {
                spdlog::debug("Key = {} value = ", it.key(), it.value());
                string value = it.value().get<std::string>();
                nodes.second.addProperty(string(it.key()), &value[0]);
            }
        }

        std::string wsData = std::to_string(pe[0].first) + " " + std::to_string(pe[0].second) + " " +
                             std::to_string(pe[1].first) + " " + std::to_string(pe[1].second);
    }
    nm->close();
    delete nm;
    graphPartitioner.printStats();
#ifdef ENABLEWS
    edgesWSServer.broadcast(wsData);

    edgesWSServer.closeAll();
    edgesWSServer.m_server.stop();
    senderThread.join();
#endif

    cout << "Time taken = " << 1000 * 1000 * (double)(clock() - start) / CLOCKS_PER_SEC << " micro seconds" << endl;
}