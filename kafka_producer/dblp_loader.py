#! /usr/bin/env python3
from kafka import KafkaProducer
from time import sleep
from datetime import datetime
from itertools import combinations
import socket
import json
import logging

logging.basicConfig(
    level=logging.INFO,
    format='%(levelname)s: %(asctime)s %(message)s',
    datefmt='%m/%d/%Y %I:%M:%S %p',
    handlers=[
        logging.FileHandler("dblp_data_publisher.log"),
        logging.StreamHandler()
    ])


topicName = "testtopic"
producer = KafkaProducer(bootstrap_servers='localhost:9092')
payloadTemplate = {
    "source": {
        "id": "",
        "properties": {
        }
    },
    "destination": {
        "id": "",
        "properties": {
        }
    },
    "properties": {

    }
}


def file_feeder(limit=3):
    edges_count = 0
    with open("../data_sets/dblp/sample_sblp.10000.txt", 'r') as data_fd:
        for line in data_fd:
            logging.debug("Current line data = {}".format(line))
            edges_count += 1
            if edges_count > limit:
                break
            publication = json.loads(line)
            authors = list(publication["authors"])
            logging.info(
                "Processing publication id = {}".format(publication["id"]))
            co_authors_count = 0
            for co_authors in combinations(authors, 2):
                source = co_authors[0]
                destination = co_authors[1]
                payload_template = {
                    "source": {
                        "id": source["id"],
                        "properties": {
                            "name": source["name"]
                        }
                    },
                    "destination": {
                        "id": destination["id"],
                        "properties": {
                            "name": source["name"]
                        }
                    },
                    "properties": {
                        "publication": publication["id"],
                        "title": publication["title"]
                    }
                }
                co_authors_count += 1
                producer.send(topicName, json.dumps(payload_template).encode())
            logging.info(
                "Submitted {} of co-author pairs".format(co_authors_count))


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
    file_feeder()
    producer.send(topicName, "-1".encode())
    producer.close()


if __name__ == "__main__":
    main()
