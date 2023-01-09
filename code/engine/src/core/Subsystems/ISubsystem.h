#include "defines.h"
#include "core/Logger/Logger.h"
#include "core/DataStructs/Strings.h"


namespace ERI
{
    class ISubsystem
    {
    public:
        ISubsystem() = default;
        virtual ~ISubsystem() = default;

        virtual b8 init(Logger *log) = 0;
        virtual void shutdown(void) = 0;
        virtual String get_name(void) = 0;

    };

}