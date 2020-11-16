
#include <spdlog/spdlog.h>

#include "libs/restAPI/GraphEndpoint.cpp"
#include "libs/graphStore/NodeManager.h"

using namespace std;

int main(int argc, char* argv[]) {
    Port port(9080);
    spdlog::set_level(spdlog::level::debug);
    NodeManager* nm = new NodeManager("app");

    spdlog::info("Starting streaming graph store REST API . . ");
    int thr = 2;

    if (argc >= 2) {
        port = static_cast<uint16_t>(std::stol(argv[1]));

        if (argc == 3) thr = std::stoi(argv[2]);
    }

    Address addr(Ipv4::any(), port);
    spdlog::info("Available hardware CPU cores = {}", hardware_concurrency());
    spdlog::info("Number of threads in use for REST API = {}", thr);

    GraphEndpoint gEP(addr, nm);

    gEP.init(thr);
    gEP.start();
}