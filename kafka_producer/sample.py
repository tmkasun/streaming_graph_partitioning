from kafka import KafkaProducer

def main():
    edges_count = 0
    topicName = "test"
    producer = KafkaProducer(bootstrap_servers='localhost:9092')
    with open("./data_sets/po.dl", 'r') as data_fd:
        for line in data_fd:
            edges = line.split(' ')
            print("{} ===> {}".format(edges[0], edges[1]))
            producer.send(topicName, line.encode())
            edges_count += 1
        print("Total edges = {} Submited to Kafka Topic name = {}".format(edges_count, topicName))


if __name__ == "__main__":
    main()
