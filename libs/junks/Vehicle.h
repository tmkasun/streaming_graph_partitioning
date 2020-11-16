#ifndef VEHICLE_CLASS
#define VEHICLE_CLASS
#include <string>

class Vehicle
{
public:
  std::string color;
  std::string getRegistrationNumber();
  Vehicle(std::string color, std::string regNumber);

private:
  std::string registration;
};

#endif