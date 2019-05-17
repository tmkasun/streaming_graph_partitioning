#include "Person.h"

string Person::getNIC()
{
    return nic;
};

Person::Person(string namea, string nica)
{
    nic = nica;
    name = namea;
};


Person::Person(){

};