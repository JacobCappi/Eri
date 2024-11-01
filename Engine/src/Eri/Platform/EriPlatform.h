#pragma once
#include "Eri/Core.h"

#include "Eri/Platform/IPlatform.h"

namespace ERI
{

class EriPlatform 
{
public:
  static IPlatform *getEriPlatform();
};

}