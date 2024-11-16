#pragma once
#include "Eri/Utils/IUtil.h"
#include "Eri/Utils/ILogger.h"
#include "Eri/Utils/IEvents.h"


#include <vector>

namespace ERI
{
/**
 * @brief Constructs all of the subsystems within ERI
 *
 */
class EriUtils : public IUtil
{
private:
  ILogger *_log;
  IEvents *_events;

  std::vector<IUtil *> _utils;

private:
  bool BuildLogger();
  bool BuildEventSystem();

public:
  EriUtils() {};
  ~EriUtils() {};

public: // IUtil
  bool Startup() override;
  bool Shutdown() override;

public:
  ILogger *getLogger() const;
  IEvents *getEventSystem() const;
};
} // namespace ERI
