
#include <cppkafka/cppkafka.h>
#include <spdlog/spdlog.h>
#include <time.h>

#include <iostream>
#include <nlohmann/json.hpp>
#include <thread>  // For running websocket server parallelly
#include "../libs/transports/http/DataPublisher.h"


using namespace std;
// for convenience
using json = nlohmann::json;

int main(int argc, char* argv[]) {
    DataPublisher workerClient(7780, "127.0.0.1");
    spdlog::set_level(spdlog::level::debug);

    std::string sampleData =
        "{ \"source\": { \"id\": \"2702511795\", \"properties\": { \"name\": \"Peter Kostelnik\" } }, \"destination\": "
        "{ \"id\": \"2041014688\", \"properties\": { \"name\": \"Peter Kostelnik\" } }, \"properties\": { "
        "\"publication\": \"100001334\", \"title\": \"Ontologies in HYDRA - Middleware for Ambient Intelligent "
        "Devices.\" } }";
    workerClient.publish(sampleData);
}