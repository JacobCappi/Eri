#pragma once

namespace ERI
{

class IUtil
{
public:
  IUtil() {};
  virtual ~IUtil() {};

public:
  virtual bool Startup() = 0;
  virtual bool Shutdown() = 0;
};

} // namespace ERI