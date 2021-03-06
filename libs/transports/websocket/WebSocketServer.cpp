/*
No copy rights , Open to do anything you wish
Based on the discussion: https://github.com/zaphoyd/websocketpp/issues/403

To install the WebSocketPP follow the steps below.
git clone https://github.com/zaphoyd/websocketpp.git
cd websocketpp
mkdir build

// Run either following 
cmake .
// Or following
cmake -DENABLE_CPP11=TRUE -DBUILD_EXAMPLES=TRUE -DBUILD_TESTS=TRUE -DBOOST_ROOT=../../boost_1_63_0

sudo make install
*/
#include "./WebSocketServer.h"

void BroadcastServer::run(uint16_t port) {
    m_server.listen(port);
    m_server.start_accept();
    std::cout << "Start listning for web socket connections from " << port << std::endl;
    m_server.run();
}

void BroadcastServer::broadcast(std::string message) {
    for (auto it : m_connections) {
        m_server.send(it, message, websocketpp::frame::opcode::text);
    }
}

void BroadcastServer::closeAll() {
    for (auto it : m_connections) {
        m_server.pause_reading(it);
        m_server.close(it,websocketpp::close::status::normal,"");
    }
}