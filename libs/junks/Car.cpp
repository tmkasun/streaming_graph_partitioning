#include "Car.h"
#include "Person.h"
#include "Vehicle.cpp"

using namespace std;
Person Car::getOwner()
{
    return owner;
};

Car::Car(Person cowner, string color, string regNumber) : Vehicle(color, regNumber)
{
    string model = "toyota";
    owner = cowner;
};

Car::Car(string thisModel) : Vehicle("Red", "Unknown")
{
    string model = thisModel;
};

Car::Car(Car &c) : Vehicle("Red", "Unknown")
{
    owner = c.owner;
    owner.name += " copied";
};