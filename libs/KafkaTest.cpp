#include <cppkafka/cppkafka.h>
#include "./headers/KafkaTest.h"

using namespace std;
using namespace cppkafka;

void KafkaTest::Subscribe(string topic){
    consumer.subscribe({topic});
}