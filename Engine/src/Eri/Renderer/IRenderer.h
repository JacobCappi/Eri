#pragma once

#include "Eri/Utils/EriUtils.h"
#include "Eri/Platform/EriPlatform.h"


namespace ERI
{

class IRenderer
{
public:
  IRenderer() {};
  virtual ~IRenderer() {};

public:
  virtual void registerUtils(EriUtils *utils) = 0;
  virtual void registerPlatform(EriPlatform *platform) = 0;
  virtual void setAppName(const char *app_name) = 0;

public:
  virtual bool Startup() = 0;
  virtual bool Shutdown() = 0;

};

} // namespace ERI


