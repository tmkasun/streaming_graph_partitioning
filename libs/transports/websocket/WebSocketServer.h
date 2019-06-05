#ifndef SAMPLE_WEBSOCKET_SERVER

#include <chrono> // For sleep
#include <iostream>
#include <set>
#include <thread>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp> // For run in server role (There are two roles, Server and Client) 

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::connection_hdl;  // https://en.cppreference.com/w/cpp/language/namespace (6)
using websocketpp::lib::bind;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

class BroadcastServer {
   public:
    typedef std::set<connection_hdl, std::owner_less<connection_hdl>> con_list;
    server m_server;
    con_list m_connections;

    BroadcastServer() {
        m_server.init_asio();

        // Handlers list : https://www.zaphoyd.com/websocketpp/manual/reference/handler-list
        m_server.set_open_handler(bind(&BroadcastServer::on_open, this, ::_1));
        m_server.set_close_handler(bind(&BroadcastServer::on_close, this, ::_1));
        
    }

    void on_open(connection_hdl hdl) { m_connections.insert(hdl); }

    void on_close(connection_hdl hdl) { m_connections.erase(hdl); }

    void run(uint16_t port);

    void broadcast(std::string message);
    void closeAll();
};

#endif // !SAMPLE_WEBSOCKET_SERVER