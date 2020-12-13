#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>

#include <algorithm>
#include <nlohmann/json.hpp>

#include "../graphStore/NodeManager.h"
using namespace Pistache;
using json = nlohmann::json;

namespace Generic {

void handleReady(const Rest::Request&, Http::ResponseWriter response) { response.send(Http::Code::Ok, "1"); }

}  // namespace Generic

class GraphEndpoint {
   public:
    explicit GraphEndpoint(Address addr, NodeManager* _nm)
        : httpEndpoint(std::make_shared<Http::Endpoint>(addr)), nm(_nm) {}

    void init(size_t thr = 2) {
        auto opts = Http::Endpoint::options().threads(static_cast<int>(thr));
        httpEndpoint->init(opts);
        setupRoutes();
    }

    void start() {
        httpEndpoint->setHandler(router.handler());
        httpEndpoint->serve();
    }

   private:
    void setupRoutes() {
        using namespace Rest;

        Routes::Post(router, "/record/:name/:value?", Routes::bind(&GraphEndpoint::doGetVertext, this));
        Routes::Get(router, "/graph/:gid/vertices/:id", Routes::bind(&GraphEndpoint::doGetVertext, this));
        Routes::Get(router, "/graph/:gid", Routes::bind(&GraphEndpoint::doGetVertices, this));
    }

    void doGetVertices(const Rest::Request& request, Http::ResponseWriter response) {
        auto name = request.param(":gid").as<std::string>();
        auto hasLimit = request.query().has("limit");

        Guard guard(metricsLock);
        std::list<NodeBlock> vetices;
        if (hasLimit) {
            std::string qLimit = request.query().get("limit").get();
            int limit = std::stoi(qLimit);
            vetices = this->nm->getGraph(limit);
        } else {
            vetices = this->nm->getGraph();
        }

        json veticesArray = json::array();
        json propertiesJson;
        for (auto it = vetices.begin(); it != vetices.end(); it++) {
            veticesArray.push_back(nodeToJson(*it));
        }
        auto mime = Http::Mime::MediaType::fromString("application/json");
        response.send(Http::Code::Ok, veticesArray.dump(), mime);
    }

    void doGetVertext(const Rest::Request& request, Http::ResponseWriter response) {
        auto vertextId = request.param(":id").as<std::string>();
        auto graphId = request.param(":gid").as<std::string>();

        Guard guard(metricsLock);
        NodeBlock* vertext = this->nm->get(vertextId);
        if (!vertext) {
            response.send(Http::Code::Not_Found, "Vertext does not exist");
        } else {
            json payload = nodeToJson(*vertext);
            auto mime = Http::Mime::MediaType::fromString("application/json");
            response.send(Http::Code::Ok, payload.dump(), mime);
        }
    }

    json nodeToJson(NodeBlock vertext) {
        auto edges = vertext.getEdges();
        auto properties = vertext.getAllProperties();
        json edgesArray = json::array();
        json propertiesJson;
        for (auto it = edges.begin(); it != edges.end(); it++) {
            edgesArray.push_back(it->id);
        }
        for (auto it = properties.begin(); it != properties.end(); it++) {
            propertiesJson[it->first] = it->second;
        }
        json payload = json::object({});
        payload["id"] = vertext.id;
        payload["properties"] = json::object({{"edges", edgesArray}, {"nodeProps", propertiesJson}});
        return payload;
    }

    using Lock = std::mutex;
    using Guard = std::lock_guard<Lock>;
    Lock metricsLock;
    NodeManager* nm = NULL;
    std::shared_ptr<Http::Endpoint> httpEndpoint;
    Rest::Router router;
};