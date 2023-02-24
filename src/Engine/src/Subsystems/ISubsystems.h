#pragma once

namespace ERI
{
    
class ISubsystems
{
public:
    ISubsystems() {};
    virtual ~ISubsystems() {};

public:
    virtual void Startup() = 0;
    virtual void Shutdown() = 0;
};

} // namespace ERI