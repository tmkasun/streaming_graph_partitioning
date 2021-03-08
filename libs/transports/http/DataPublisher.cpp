#include "./DataPublisher.h"

#include "../../../util/logger/Logger.h"

Logger data_publisher_logger;

DataPublisher::DataPublisher(int remote_port, std::string remote_address) {
    this->remote_port = remote_port;
    this->remote_address = remote_address;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(remote_port);
    if (inet_pton(AF_INET, remote_address.c_str(), &serv_addr.sin_addr) <= 0) {
        data_publisher_logger.error("Invalid address/ Address not supported!");
    }
}

void DataPublisher::publish(std::string message) {
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        data_publisher_logger.error("Socket creation error!");
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        data_publisher_logger.error("Connection Failed!");
    }
    send(this->sock, message.c_str(), message.length(), 0);
    data_publisher_logger.info("Hello message sent\n");
    close(sock);
}
