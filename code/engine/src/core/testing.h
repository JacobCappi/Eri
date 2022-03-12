#pragma once

#include <iostream>
#include <string>

class Logger
{
public:
    Logger();
    ~Logger();

public:
    bool testing_writes(std::string output);
};
