#ifndef KAFKATEST_CLASS
#define KAFKATEST_CLASS

#include <cppkafka/cppkafka.h>

class KafkaTest
{
  public:
    cppkafka::Consumer consumer;
    KafkaTest(cppkafka::Configuration configs) : consumer(configs) { _configs = configs; };
    KafkaTest();
    ~KafkaTest(){};
    void Subscribe(std::string topic);

  private:
    cppkafka::Configuration _configs;
};

#endif