#include "Eri/Utils/EriUtils.h"

#include "Eri/Utils/IUtil.h"
#include "Eri/Utils/ILogger.h"
#include "Eri/Utils/IEvents.h"

#include "Eri/Utils/Logger/BasicLogger/BasicLogger.h"
#include "Eri/Utils/Events/VectorEvents/VectorEvents.h"


namespace ERI
{

bool EriUtils::BuildLogger()
{
  _log = new BasicLogger();
  _log->SetLogLevel(false);

  _subsystems.push_back(_log);
  return true;
}

bool EriUtils::BuildEventSystem()
{
  if (_log == nullptr)
  {
      return false;
  }
  _events = new VectorEvents();
  _events->RegisterLogger(_log);
  _subsystems.push_back(_events);
  return true;
}


bool EriUtils::Startup()
{
  bool retVal = true;

  _log = nullptr;
  _events = nullptr;
  _subsystems.clear();

  if (!BuildLogger())
  {
      return false;
  }

  if (!BuildEventSystem())
  {
      return false;
  }

  for (auto system : _subsystems)
  {
      retVal &= system->Startup();
  }

  // _platform->SetWindowPosition(50, 50);
  // _platform->SetWindowSize(800, 600);
  // _platform->StartupWindow("ERI Game Engine - Windows");

  return retVal;
}

bool EriUtils::Shutdown()
{
  bool retVal = true;
  for (auto system : _subsystems)
  {
      retVal = retVal && system->Shutdown();
  }
  _subsystems.clear();

  delete _log;
  delete _events;

  _log = nullptr;
  _events = nullptr;

  return retVal;
}

ILogger* EriUtils::getLogger() const
{
  return _log;
}

IEvents* EriUtils::getEventSystem() const
{
  return _events;
}

} // namespace ERI


