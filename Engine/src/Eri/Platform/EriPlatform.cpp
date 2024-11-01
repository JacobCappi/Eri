#include "Eri/Core.h"
#include "Eri/Platform/EriPlatform.h"


#if ERI_WINDOWS

namespace ERI
{
IPlatform *EriPlatform::getEriPlatform()
{

}
} // namespace ERI
#elif ERI_LINUX

namespace ERI
{
IPlatform *EriPlatform::getEriPlatform()
{
  return nullptr;

}

} // namespace ERI
#endif
  

