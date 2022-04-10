#include "ISubsystem.h"
#include "Platform.h"


namespace ERI
{

    class Memory : protected ISubsystem
    {

    private:
        char *subsystem_name;
        Logger *log = nullptr;

    protected:
        void print_name(std::ostream& str) const;

    public:
        Memory() = default;
        ~Memory() = default;
        b8 init(Logger *log);
        void shutdown(void);

        malloc();
        //todo: see if this works
        new();
    };

}