from kafka import KafkaProducer
from time import sleep
from datetime import datetime
import sys
import socket
import json

topicName = "testtopic"
producer = KafkaProducer(bootstrap_servers='localhost:9092')
payloadTemplate = {
    "source": {
        "id": "",
        "properties": {
            "fname": "Kasun"
        }
    },
    "destination": {
        "id": "",
        "properties": {
            "lname": "Thennakoon"
        }
    }
}


def file_feeder(limit=10):
    edges_count = 0
    with open("../data_sets/po.dl", 'r') as data_fd:
        for line in data_fd:
            if edges_count > limit:
                break
            nodes = line.strip().split(' ')
            print("{} ===> {}".format(nodes[0], nodes[1]))
            payloadTemplate["source"]["id"] = nodes[0]
            payloadTemplate["destination"]["id"] = nodes[1]
            timestamp = datetime.now().strftime("%A, %d. %B %Y %I:%M%p")
            payloadTemplate["source"]["properties"]["timestamp"] = timestamp
            payloadTemplate["destination"]["properties"]["timestamp"] = timestamp
            print(json.dumps(payloadTemplate))
            producer.send(topicName, json.dumps(payloadTemplate).encode())
            edges_count += 1
            # sleep(0.3)
            # if edges_count == 200:
            #     break
    print("Total edges = {} Submitted to Kafka Topic name = {}".format(
        edges_count, topicName))


def rawInputs():
    edges_count = 0
    userIn = input("Enter edge pair:")
    while userIn != "-1":
        edges = userIn.split(' ')
        print("{} ===> {}".format(edges[0], edges[1]))
        producer.send(topicName, userIn.encode())
        edges_count += 1
        userIn = input("Enter edge pair:")
    print("Total edges = {} Submited to Kafka Topic name = {}".format(
        edges_count, topicName))


def initJasmineStream():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_connection = ('localhost', 7777)
    sock.connect(server_connection)
    sock.sendall(b'adstrmk')
    data = ""
    while data.find("\r\n") < 0:
        bdata = sock.recv(16)
        data += bdata.decode()
    sock.sendall(topicName.encode())


def main():
    # initJasmineStream()
    try:
        feederType = sys.argv[1]
    except IndexError:
        feederType = None
    print("Getting inputs from = {}".format(feederType))
    if feederType == "file":
        file_feeder()
    else:
        rawInputs()
    producer.send(topicName, "-1".encode())
    producer.close()


if __name__ == "__main__":
    main()
