#include "defines.h"
#include "core/Logger/Logger.h"

//TODO: let get_name return custom string lib
#include <string>


namespace ERI
{
    class ISubsystem
    {
    protected:
        virtual void set_subsystem_name(const std::string name) = 0;

    public:
        ISubsystem() = default;
        virtual ~ISubsystem(void) = default;

        virtual b8 init(Logger *log) = 0;
        virtual void shutdown(void) = 0;
        virtual const std::string get_name(void) = 0;
    };

}