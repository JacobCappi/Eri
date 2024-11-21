#include "Eri/Core.h"
#include "Eri/Platform/EriPlatform.h"


namespace ERI
{

bool EriPlatform::Startup()
{
  _platform = nullptr;
  return true;
}

bool EriPlatform::Shutdown()
{
  _platform->Shutdown();
  delete(_platform);
  return true;
}

} // namespace ERI


#if ERI_WINDOWS

#include "Eri/Platform/Windows/PlatformWindows.h"

namespace ERI
{
IPlatform *EriPlatform::getEriPlatform()
{
  if (_platform == nullptr)
  {
    _platform = new PlatformWindows();
  }
  return _platform;

}

Platforms EriPlatform::platformType()
{
  return Platforms::WINDOWS;
}

} // namespace ERI
#elif ERI_LINUX

#include "Eri/Platform/Linux/PlatformLinux.h"

namespace ERI
{
IPlatform *EriPlatform::getEriPlatform()
{
  if (_platform == nullptr)
  {
    _platform = new PlatformLinux();
  }
  return _platform;
}


Platforms EriPlatform::platformType()
{
  return Platforms::LINUX_X11;
}

} // namespace ERI

#else

namespace ERI
{
IPlatform *EriPlatform::getEriPlatform()
{
  return nullptr;
}


Platforms EriPlatform::platformType()
{
  return Platforms::UNSUPPORTED;
}

} // namespace ERI

#endif


