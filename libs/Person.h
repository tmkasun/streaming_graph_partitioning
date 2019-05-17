#ifndef PERSON_CLASS
#define PERSON_CLASS
#include <string>
#include <vector>

class Person
{

public:
  std::string name;
  int age;
  std::vector<std::string> ownedCars;
  std::string getNIC();
  Person(std::string name, std::string nic);
  Person();

private:
  std::string nic;
};

#endif