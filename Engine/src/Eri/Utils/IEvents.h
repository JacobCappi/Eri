#pragma once
#include "Eri/Core.h"

#include "Eri/Utils/IUtil.h"
#include "Eri/Utils/ILogger.h"

#include "Eri/Utils/Events/KeyPress/IKeyPressSubscriber.h"
#include "Eri/Utils/Events/Mouse/IMouseSubscriber.h"
#include "Eri/Utils/Events/WindowState/IWindowStateSubscriber.h"


namespace ERI
{

// TODO: maybe interfaces (IPublisher, ISubscriber, etc..?)
class IEvents : public IUtil
{

public:
  IEvents() {};
  virtual ~IEvents() {};

public:
  virtual void RegisterLogger(ILogger *log) = 0;

public:
  virtual u64 SubscribeKeyPress(IKeyPressSubscriber * const) = 0;
  virtual u64 SubscribeMouse(IMouseSubscriber * const) = 0;
  virtual u64 SubscribeWindowState(IWindowStateSubscriber * const) = 0;

  virtual bool UnsubscribeKeyPress(u64 id) = 0;
  virtual bool UnsubscribeMouse(u64 id) = 0;
  virtual bool UnsubscribeWindowState(u64 id) = 0;

  virtual bool PublishKeyPress(enum Keys, enum KeyPressType) const = 0;
  virtual bool PublishMouse(enum Mouse, i32 x, i32 y) const = 0;
  virtual bool PublishWindowState(enum WindowState, i32 l, i32 r) const = 0;

};

} // namespace ERI