#include <cppkafka/cppkafka.h>
#include <time.h>
#include <iostream>
#include <thread> // FOr running websocket server parallelly

#include "libs/Partition.h"
#include "libs/Partitioner.h"
#include "libs/headers/KafkaTest.h"
#include "libs/transports/websocket/WebSocketServer.h"

using namespace std;

int main(int argc, char *argv[]) {
    clock_t start = clock();
    Partitioner graphPartitioner(4);
    uint16_t websocketPort = 9002;
    BroadcastServer edgesWSServer;
    std::thread senderThread(std::bind(&BroadcastServer::run, &edgesWSServer, websocketPort));

    bool DEBUG = true;
    if (argc > 1 && *argv[1] == 'd') DEBUG = true;
    cppkafka::Configuration configs = {{"metadata.broker.list", "127.0.0.1:9092"}, {"group.id", "knnect"}};
    // cppkafka::Consumer cons(configs);
    KafkaTest ktesting(configs);

    ktesting.Subscribe("test");

    while (true) {
        if (DEBUG) cout << "Waiting to receive message. . ." << endl;
        cppkafka::Message msg = ktesting.consumer.poll();
        if (DEBUG) cout << "Got a message" << endl;
        if (!msg) {
            if (DEBUG) cout << "Not a message!" << endl;
            continue;
        }

        if (msg.get_error()) {
            if (DEBUG) cout << "Error in message!" << endl;

            if (msg.is_eof()) {
                cout << "Message end of file received!" << endl;
            }
            continue;
        }

        cout << "Received message from topic = " << msg.get_topic() << endl;
        cout << "Partition ID = " << msg.get_partition() << endl;
        cout << "Offset = " << msg.get_offset() << endl;
        string data(msg.get_payload());
        cout << "Payload = " << data << endl;
        edgesWSServer.broadcast(data);
        if (data == "-1") {  // Marks the end of stream
            cout << " ***Received the end of stream" << endl;
            break;
        }
        std::pair<long, long> edge = Partitioner::deserialize(data);
        graphPartitioner.addEdge(edge);
    }
    graphPartitioner.printStats();
    edgesWSServer.m_server.stop();
    senderThread.join();
    cout << "Time taken = " << 1000 * 1000 * (double)(clock() - start) / CLOCKS_PER_SEC << " micro seconds" << endl;
}