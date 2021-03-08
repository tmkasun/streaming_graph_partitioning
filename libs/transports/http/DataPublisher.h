#ifndef WORKER_DATA_PUBLISHER

#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

class DataPublisher {
   private:
    int sock = 0, valread, remote_port;
    struct sockaddr_in serv_addr;
    std::string remote_address, message;
    char buffer[1024] = {0};

   public:
    DataPublisher(int, std::string);
    void publish(std::string);
};

#endif  // !Worker_data_publisher
