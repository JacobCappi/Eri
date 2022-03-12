#include "testing.h"
#include <iostream>

Logger::Logger()
{
    std::cout << "Logger init" << std::endl;
}

Logger::~Logger()
{
    std::cout << "Logger off" << std::endl;
}

bool Logger::testing_writes(std::string output)
{
    std::cout << "Message: " << output << std::endl;
    return true;
}