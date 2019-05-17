#ifndef CAR_CLASS
#define CAR_CLASS
#include <string>
#include "Person.h"
#include "Vehicle.h"

class Car : public Vehicle
{

public:
  std::string number;
  Person owner;
  std::string type;
  Person getOwner();
  Car(Person cowner, std::string color, std::string regNumber);
  Car(std::string model);
  Car(Car &c);
};

#endif