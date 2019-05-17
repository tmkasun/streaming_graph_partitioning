#include "Vehicle.h"
#include <string>
#include <iostream>

using namespace std;

Vehicle::Vehicle(string ncolor, string regNumber)
{
  cout << "Calling Vechile constructor with ncolor = " << ncolor << " regNumber = " << regNumber << endl;
  color = ncolor;
  registration = regNumber;
};

string Vehicle::getRegistrationNumber()
{
  return registration;
}