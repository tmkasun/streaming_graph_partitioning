#include <cppkafka/cppkafka.h>
#include <time.h>
#include <iostream>

#include "libs/Partition.h"
#include "libs/headers/KafkaTest.h"

using namespace std;

template <typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

int main(int argc, char *argv[]) {
    clock_t start = clock();
    vector<Partition> partitions;
    for (size_t i = 0; i < 4; i++) partitions.push_back(Partition(100));

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
        std::vector<string> v = split(data, ' ');
        cout << "Vertext 1 = " << stoi(v[0]) << endl;
        cout << "Vertext 2 = " << stoi(v[1]) << endl;
        partitions[1].addEdge({stoi(v[0]), stoi(v[1])});
        cout << "Vertext count = " << partitions[1].vertextCount() << endl;
        cout << "Edges count = " << partitions[1].edgesCount() << endl;
    }

    cout << "Time taken = " << 1000 * 1000 * (double)(clock() - start) / CLOCKS_PER_SEC << " micro seconds" << endl;
}

// void fennel(const Graph &graph, int partitions_number, Partition *partition) {
//     const double gamma = 3 / 2.0;
//     const double alfa =
//         graph.get_edges_number() * pow(partitions_number, gamma - 1.0) / pow(graph.get_nodes_number(), gamma);

//     // printf("gamma=%f alfa=%f\n",gamma,alfa);

//     const bool with_threshold = true;
//     const double v = 1.1;

//     /*
//   if (with_threshold)
//   printf("With threshold set to %f\n",v);
// else
// printf("Without threshold\n");*/

//     vector<int> nodes;
//     vector<double> partition_score(partitions_number);

//     // get random sorted nodes
//     graph.get_nodes_randomly(&nodes);

//     // initialize partitions
//     partition->set_size(nodes.size(), partitions_number);

//     int count = 0;

//     for (vector<int>::iterator node = nodes.begin(); node != nodes.end(); ++node) {
//         // initialize partitions score
//         for (int i = 0; i < partitions_number; i++) partition_score[i] = 0;

//         int nn = 0;
//         // add to partitions score number of neighbors in partition
//         for (vector<int>::const_iterator neighbor = graph.get_neighbors(*node).begin();
//              neighbor != graph.get_neighbors(*node).end(); ++neighbor) {
//             int neighbor_partition = partition->get_node_partition(*neighbor);
//             if (neighbor_partition >= 0) {
//                 partition_score[neighbor_partition]++;
//                 // nn++;
//             }
//         }

//         count++;
//         if (nn > 0) printf("iteration %d neigh %d\n", count, nn);

//         // subtract to partitions score cost function
//         for (int i = 0; i < partitions_number; i++) {
//             int partition_size = partition->get_partition_size(i);
//             double c = alfa * (pow(partition_size + 1, gamma) - pow(partition_size, gamma));
//             // double c = alfa * gamma * pow(partition_size,gamma-1);
//             if (nn > 0) {
//                 printf("partition %d size %d\n", i, partition_size);
//                 printf("partition %d score %f\n", i, partition_score[i]);
//             }
//             partition_score[i] -= c;
//             if (nn > 0) printf("partition %d score %f\n", i, partition_score[i]);
//         }
//         // printf("\n\n");

//         // filter out partition above treshold
//         vector<pair<double, int>> filtered_partition_score;
//         for (int i = 0; i < partitions_number; i++) {
//             // printf("Partition %d, score %f\n",i,partition_score[i]);
//             if (partition->get_partition_size(i) <= v * graph.get_nodes_number() / ((double)partitions_number) ||
//                 !with_threshold) {
//                 filtered_partition_score.push_back(pair<double, int>(partition_score[i], i));
//             }
//         }

//         // select best partition
//         int selected_partition = filtered_partition_score[0].second;
//         double score = filtered_partition_score[0].first;
//         for (size_t i = 1; i < filtered_partition_score.size(); i++) {
//             if (filtered_partition_score[i].first > score) {
//                 selected_partition = filtered_partition_score[i].second;
//                 score = filtered_partition_score[i].first;
//             }
//         }

//         if (nn > 0) printf("Selected partition %d\n", selected_partition);

//         // assign node to partition
//         partition->set_node_partition(*node, selected_partition);
//     }