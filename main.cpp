#include <iostream>
#include <typeinfo>
#include <stdlib.h>
#include <cmath>
#include <array>
#include <vector>
#include <time.h>
#include <cctype>
#include <pwd.h>
#include <unistd.h>
#include <bits/stdc++.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cppkafka/cppkafka.h>

#include "libs/FileHandler.h"
#include "libs/headers/KafkaTest.h"
#include "libs/myFunctions.h"
#include "libs/NameSpaceTest2.h"
#include "libs/NameSpaceTest.h"
#include "libs/Car.h"
#include "libs/Person.h"

using namespace std;

class Cat
{
  public:
    string name;
    Person *owner;
    Cat(string n, Person *o)
    {
        name = n;
        owner = o;
    }
};

string getHomeDir();
bool createDir(string path)
{
    return mkdir(path.c_str(), 0777) != -1;
}

int main(int argc, char *argv[])
{
    clock_t start = clock();

    bool DEBUG = true;
    if (argc > 1 && *argv[1] == 'd')
        DEBUG = true;
    cppkafka::Configuration configs = {
        {"metadata.broker.list", "127.0.0.1:9092"}, {"group.id", "knnect"}};
    // cppkafka::Consumer cons(configs);
    KafkaTest ktesting(configs);

    ktesting.Subscribe("test");

    while (true)
    {
        if (DEBUG)
            cout << "Waiting to receive message. . ." << endl;
        cppkafka::Message msg = ktesting.consumer.poll();
        if (DEBUG)
            cout << "Got a message" << endl;
        if (!msg)
        {
            if (DEBUG)
                cout << "Not a message!" << endl;
            continue;
        }

        if (msg.get_error())
        {
            if (DEBUG)
                cout << "Error in message!" << endl;

            if (msg.is_eof())
            {
                cout << "Message end of file received!" << endl;
            }
            continue;
        }

        cout << "Received message on partition " << msg.get_topic() << "/" <<
        msg.get_partition() << ", offset " << msg.get_offset() << endl; cout
        << "Payload = " << msg.get_payload() << endl;
    }

    // FileHandler testFile("/tmp/ok");

    // cout << testFile.isAvailable() << endl;
    // cout << getHomeDir() << endl;

    // string path = "/tmp/wola/";
    // cout << "Creating directory = " << path << endl;
    // cout << createDir(path) << endl;

    // path = "/tmp/wola/";
    // cout << "Creating directory = " << path << endl;
    // cout << createDir(path) << endl;

    // path = "/tmp/laldu/";
    // cout << "Creating directory = " << path << endl;
    // cout << createDir(path) << endl;

    // path = "/tmp/laldu/patta/";
    // cout << "Creating directory = " << path << endl;
    // cout << createDir(path) << endl;

    // path = "/tmp/noslash";
    // cout << "Creating directory = " << path << endl;
    // cout << createDir(path) << endl;

    // Person me("Kasun bandara Thennakoon", "902621407V");
    // me.age = 24;

    // Cat c1("Tasa", &me);

    // cout << "wolaaaaaaaaaaaa" << c1.owner->name << endl;
    // Cat c2 = c1;
    // c2.owner->name = "todaa";
    // cout << "wolaaaaaaaaaaaa" << c1.owner->name << endl;

    // cout << "wola22222222aaaaaaa" << c2.owner->name << endl;

    // Car alto(me, "Granite Gray", "CAF-8775");
    // cout << "color = " << alto.color << endl;
    // cout << "reg number = " << alto.getRegistrationNumber() << endl;

    // Car bulto = alto;
    // bulto.owner.name = "Dasun";

    // cout << "alto owner NIC = " << alto.getOwner().getNIC() << endl;
    // cout << "bulto owner NIC = " << bulto.getOwner().getNIC() << endl;

    cout << "Time taken = " << 1000 * 1000 * (double)(clock() - start) /
    CLOCKS_PER_SEC << " micro seconds" << endl;
}

string getHomeDir()
{
    const char *homedir;
    if ((homedir = getenv("HOMEA")) == NULL)
    {
        homedir = getpwuid(getuid())->pw_dir;
    }
    return string(homedir);
}


// #include <iostream>
// #include <vector>

// using namespace std;
// int main() {
//     int *a;
//     int b;
//     a = &b;
//     b = 123;

//     cout << "Sample Test " << b << endl;
//     cout << "Sample Test " << &b << endl;
//     cout << "Sample Test " << a << endl;
//     b += 30;
//     cout << "Sample Test " << *a << endl;
//     cout << "Sample Test " << &a << endl;
//     return 2;
// }

// string getname(string &name) { return "Wola" + name; }

void fennel(const Graph &graph, int partitions_number, Partition *partition) {
    const double gamma = 3 / 2.0;
    const double alfa =
        graph.get_edges_number() * pow(partitions_number, gamma - 1.0) / pow(graph.get_nodes_number(), gamma);

    // printf("gamma=%f alfa=%f\n",gamma,alfa);

    const bool with_threshold = true;
    const double v = 1.1;

    /*
  if (with_threshold)
  printf("With threshold set to %f\n",v);
else
printf("Without threshold\n");*/

    vector<int> nodes;
    vector<double> partition_score(partitions_number);

    // get random sorted nodes
    graph.get_nodes_randomly(&nodes);

    // initialize partitions
    partition->set_size(nodes.size(), partitions_number);

    int count = 0;

    for (vector<int>::iterator node = nodes.begin(); node != nodes.end(); ++node) {
        // initialize partitions score
        for (int i = 0; i < partitions_number; i++) partition_score[i] = 0;

        int nn = 0;
        // add to partitions score number of neighbors in partition
        for (vector<int>::const_iterator neighbor = graph.get_neighbors(*node).begin();
             neighbor != graph.get_neighbors(*node).end(); ++neighbor) {
            int neighbor_partition = partition->get_node_partition(*neighbor);
            if (neighbor_partition >= 0) {
                partition_score[neighbor_partition]++;
                // nn++;
            }
        }

        count++;
        if (nn > 0) printf("iteration %d neigh %d\n", count, nn);

        // subtract to partitions score cost function
        for (int i = 0; i < partitions_number; i++) {
            int partition_size = partition->get_partition_size(i);
            double c = alfa * (pow(partition_size + 1, gamma) - pow(partition_size, gamma));
            // double c = alfa * gamma * pow(partition_size,gamma-1);
            if (nn > 0) {
                printf("partition %d size %d\n", i, partition_size);
                printf("partition %d score %f\n", i, partition_score[i]);
            }
            partition_score[i] -= c;
            if (nn > 0) printf("partition %d score %f\n", i, partition_score[i]);
        }
        // printf("\n\n");

        // filter out partition above treshold
        vector<pair<double, int>> filtered_partition_score;
        for (int i = 0; i < partitions_number; i++) {
            // printf("Partition %d, score %f\n",i,partition_score[i]);
            if (partition->get_partition_size(i) <= v * graph.get_nodes_number() / ((double)partitions_number) ||
                !with_threshold) {
                filtered_partition_score.push_back(pair<double, int>(partition_score[i], i));
            }
        }

        // select best partition
        int selected_partition = filtered_partition_score[0].second;
        double score = filtered_partition_score[0].first;
        for (size_t i = 1; i < filtered_partition_score.size(); i++) {
            if (filtered_partition_score[i].first > score) {
                selected_partition = filtered_partition_score[i].second;
                score = filtered_partition_score[i].first;
            }
        }

        if (nn > 0) printf("Selected partition %d\n", selected_partition);

        // assign node to partition
        partition->set_node_partition(*node, selected_partition);
    }