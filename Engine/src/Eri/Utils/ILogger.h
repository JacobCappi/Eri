#pragma once
#include "Eri/Utils/IUtil.h"

namespace ERI
{


/**
 * @brief Interface that defines all properties of a supported ERI Logger
 */
class ILogger : public IUtil
{

public:
  ILogger() {};
  virtual ~ILogger() {};

public:
  virtual void LogInfo(const char *, ...) = 0;
  virtual void LogDebug(const char *, ...) = 0;
  virtual void LogTrace(const char *, ...) = 0;
  virtual void LogWarning(const char *, ...) = 0;
  virtual void LogError(const char *, ...) = 0;

  /**
   * @brief Sets whether the logger is in release mode
   *
   * @param isRelease true if release, false otherwise
   */
  virtual void SetLogLevel(const bool isRelease) = 0;
};

} // namespace ERI