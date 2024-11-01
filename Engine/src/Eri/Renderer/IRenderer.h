#pragma once

#include "Eri/Utils/EriUtils.h"


namespace ERI
{

class IRenderer
{
public:
  IRenderer() {};
  virtual ~IRenderer() {};

public:
  virtual void registerUtils(EriUtils *utils) = 0;
  virtual void registerPlatform(IPlatform *platform) = 0;
  virtual void setAppName(const char *app_name) = 0;
};

} // namespace ERI


