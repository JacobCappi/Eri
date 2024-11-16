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

  _utils.push_back(_log);
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
  _utils.push_back(_events);
  return true;
}


bool EriUtils::Startup()
{
  bool retVal = true;

  _log = nullptr;
  _events = nullptr;
  _utils.clear();

  if (!BuildLogger())
  {
      return false;
  }

  if (!BuildEventSystem())
  {
      return false;
  }

  for (auto system : _utils)
  {
      retVal &= system->Startup();
  }

  return retVal;
}

bool EriUtils::Shutdown()
{
  bool retVal = true;
  for (auto system : _utils)
  {
      retVal = retVal && system->Shutdown();
  }
  _utils.clear();

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


