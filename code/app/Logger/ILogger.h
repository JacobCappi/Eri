#pragma once

// Ideally, LogWarning("This log %d", memory) should work
// LogWarning("{} {} ", memory) I think works in new C++?

// TODO: should I have basic logger be a type subsystem and logger, or logger that is a subsystem (?)
class ILogger  : public ISubsystem
{
public:
    // TODO: If I pass in a std::string, how do I get formatting?
    virtual void LogWarning();
};

