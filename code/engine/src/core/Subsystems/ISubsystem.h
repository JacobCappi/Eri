#include "defines.h"
#include "core/Logger/Logger.h"

//TODO: let get_name return custom string lib
#include <iostream>


namespace ERI
{
    class ISubsystem
    {
    protected:
        virtual void print_name(std::ostream& str) const = 0;

    public:
        ISubsystem() = default;
        virtual ~ISubsystem(void) = default;

        virtual b8 init(Logger *log) = 0;
        virtual void shutdown(void) = 0;

        friend std::ostream& operator<<(std::ostream& os, const ISubsystem& system)
        {
            system.print_name(os);
            return os;
        }

    };

}