from kafka import KafkaProducer
from time import sleep
import sys

topicName = "test"
producer = KafkaProducer(bootstrap_servers='localhost:9092')
    
def fileFeed():
    edges_count = 0
    with open("./data_sets/po.dl", 'r') as data_fd:
        for line in data_fd:
            edges = line.split(' ')
            print("{} ===> {}".format(edges[0], edges[1]))
            producer.send(topicName, line.encode())
            edges_count += 1
            if edges_count == 20:
                break
    print("Total edges = {} Submited to Kafka Topic name = {}".format(edges_count, topicName))

def rawInputs():
    edges_count = 0
    userIn = input("Enter edge pair:")
    while userIn != "-1":
        edges = userIn.split(' ')
        print("{} ===> {}".format(edges[0], edges[1]))
        producer.send(topicName, userIn.encode())
        edges_count += 1
        userIn = input("Enter edge pair:")
    print("Total edges = {} Submited to Kafka Topic name = {}".format(edges_count, topicName))

def main():
    try:
        feederType = sys.argv[1]
    except IndexError:
        feederType = None
    print("Getting inputs from = {}".format(feederType))
    if feederType == "file":
        fileFeed()
    else:
        rawInputs()
    # rawInputs()
    producer.send(topicName, "-1".encode())
    producer.close()


if __name__ == "__main__":
    main()
