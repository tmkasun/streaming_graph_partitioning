#include "FileHandler.h"

using namespace std;

bool FileHandler::isAvailable()
{
    ifstream myfile(this->file);
    return myfile.good();
};
