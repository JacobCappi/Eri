#pragma once

namespace ERI
{

class ISubsystems
{
public:
    ISubsystems() {};
    virtual ~ISubsystems() {};

public:
    virtual bool Startup() = 0;
    virtual bool Shutdown() = 0;
};

} // namespace ERI