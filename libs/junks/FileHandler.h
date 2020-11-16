#ifndef FILE_HANDLER_CLASS
#define FILE_HANDLER_CLASS
#include <fstream>
#include <string>

class FileHandler
{
  public:
    std::string file;
    bool isAvailable();
    FileHandler(std::string file) : file(file){};
};

#endif