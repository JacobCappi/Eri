#pragma once

#include "Eri/Core.h"

#include "Eri/Utils/IUtil.h"
#include "Eri/Utils/IEvents.h"
#include "Eri/Utils/ILogger.h"

#include "Eri/Utils/Events/KeyPress/IKeyPressSubscriber.h"
#include "Eri/Utils/Events/Mouse/IMouseSubscriber.h"
#include "Eri/Utils/Events/WindowState/IWindowStateSubscriber.h"



#include <vector>

namespace ERI
{
class ERI_API VectorEvents : public IEvents
{

private:
  ILogger *_log;

private:
  u32 _keyPressCount = 0;
  u32 _mouseCount = 0;
  u32 _internalCount = 0;

private:
  std::vector<IKeyPressSubscriber *> _keyPressSubscribers;
  std::vector<IMouseSubscriber *> _mouseSubscribers;
  std::vector<IWindowStateSubscriber *> _windowStateSubscribers;

public:
  VectorEvents() {};
  ~VectorEvents() {};

public: // ISubsystem
  bool Startup() override;
  bool Shutdown() override;

public: // IEvents
  u64 SubscribeKeyPress(IKeyPressSubscriber *const sub) override;
  u64 SubscribeMouse(IMouseSubscriber *const sub) override;
  u64 SubscribeWindowState(IWindowStateSubscriber *const sub) override;

  bool UnsubscribeKeyPress(u64 id) override;
  bool UnsubscribeMouse(u64 id) override;
  bool UnsubscribeWindowState(u64 id) override;

  bool PublishKeyPress(enum Keys, enum KeyPressType) const override;
  bool PublishMouse(enum Mouse, i32 x, i32 y) const override;
  bool PublishWindowState(enum WindowState, i32 l, i32 r) const override;

  void RegisterLogger(ILogger *log) override;

// TOOD: should I find a better place for this?
public:
  void LogPress(enum Keys) const override;
  void LogMouse(enum Mouse, i32 x, i32 y) const override;
  void LogWindowState(enum WindowState, i32 l, i32 r) const override;
};
} // namespace ERI
