#include "Eri/Utils/Events/VectorEvents/VectorEvents.h"


// TODO: this *could* waste lots of space if sub/unsub is in a loop
// Don't do this for now

namespace ERI
{
bool VectorEvents::Startup()
{
  if (_log == nullptr)
  {
      return false;
  }

  _log->LogInfo("Events System type Vector is starting up");
  // TODO: preallocate for optimization
  return true;
}

bool VectorEvents::Shutdown()
{
  _log->LogInfo("Events System type Vector is shutting down");
  _log = nullptr;

  return true;
}

u64 VectorEvents::SubscribeKeyPress(IKeyPressSubscriber *const sub)
{
  auto index = _keyPressSubscribers.begin() + _keyPressCount;
  _keyPressSubscribers.insert(index, sub);

  _log->LogDebug("Subscriber %d added to KeyPress", _keyPressCount);
  return _keyPressCount++;
}

u64 VectorEvents::SubscribeMouse(IMouseSubscriber *const sub)
{
  auto index = _mouseSubscribers.begin() + _mouseCount;
  _mouseSubscribers.insert(index, sub);

  _log->LogDebug("Subscriber %d added to Mouse", _mouseCount);
  return _mouseCount++;
}

u64 VectorEvents::SubscribeWindowState(IWindowStateSubscriber *const sub)
{
  auto index = _windowStateSubscribers.begin() + _internalCount;
  _windowStateSubscribers.insert(index, sub);

  _log->LogDebug("Subscriber %d added to Internal", _internalCount);
  return _internalCount++;
}

bool VectorEvents::UnsubscribeKeyPress(u64 id)
{
  auto index = _windowStateSubscribers.begin() + id;
  if (index >= _windowStateSubscribers.end())
  {
      _log->LogDebug("Subscriber %d not found in Key Press", id);
      return false;
  }


  _log->LogDebug("Subscriber %d removed from Key Press", id);
  // this *is* costly I think... we'll make a map change sometime
  _windowStateSubscribers.erase(index);
  return true;
}

bool VectorEvents::UnsubscribeMouse(u64 id)
{
  auto index = _mouseSubscribers.begin() + id;
  if (index >= _mouseSubscribers.end())
  {
    _log->LogDebug("Subscriber %d not found in Mouse", id);
    return false;
  }


  _log->LogDebug("Subscriber %d removed from Mouse", id);
  // this *is* costly I think... we'll make a map change sometime
  _mouseSubscribers.erase(index);
  return true;
}

bool VectorEvents::UnsubscribeWindowState(u64 id)
{
  auto index = _windowStateSubscribers.begin() + id;
  if (index >= _windowStateSubscribers.end())
  {
    _log->LogDebug("Subscriber %d not found in Internal", id);
    return false;
  }


  _log->LogDebug("Subscriber %d removed from Internal", id);
  // this *is* costly I think... we'll make a map change sometime
  _windowStateSubscribers.erase(index);
  return true;
}

bool VectorEvents::PublishKeyPress(enum Keys key, enum KeyPressType keyPressType) const
{
  if (_keyPressSubscribers.empty())
  {
    return false;
  }

  for (auto subscribers : _keyPressSubscribers)
  {
    subscribers->onKeyEvent(keyPressType, key);
  }

  return true;
}

bool VectorEvents::PublishMouse(enum Mouse mouse, i32 x, i32 y) const
{
  if (_mouseSubscribers.empty())
  {
    return false;
  }

  for (auto subscribers : _mouseSubscribers)
  {
    subscribers->onMouseEvent(mouse, x, y);
  }

  return true;
}

bool VectorEvents::PublishWindowState(enum WindowState state, i32 left, i32 right) const
{
  if (_windowStateSubscribers.empty())
  {
    return false;
  }

  for (auto subscribers : _windowStateSubscribers)
  {
    subscribers->onWindowStateEvent(state, left, right);
  }

  return true;
}

void VectorEvents::RegisterLogger(ILogger *log)
{
  _log = log;
}

void VectorEvents::LogPress(enum Keys key) const
{
  switch (key)
  {
    case Keys::A:
      _log->LogInfo("Key A");
      break;
    case Keys::B:
      _log->LogInfo("Key B");
      break;
    case Keys::C:
      _log->LogInfo("Key C");
      break;
    case Keys::D:
      _log->LogInfo("Key D");
      break;
    case Keys::E:
      _log->LogInfo("Key E");
      break;
    case Keys::F:
      _log->LogInfo("Key F");
      break;
    case Keys::G:
      _log->LogInfo("Key G");
      break;
    case Keys::H:
      _log->LogInfo("Key H");
      break;
    case Keys::I:
      _log->LogInfo("Key I");
      break;
    case Keys::J:
      _log->LogInfo("Key J");
      break;
    case Keys::K:
      _log->LogInfo("Key K");
      break;
    case Keys::L:
      _log->LogInfo("Key L");
      break;
    case Keys::M:
      _log->LogInfo("Key M");
      break;
    case Keys::N:
      _log->LogInfo("Key N");
      break;
    case Keys::O:
      _log->LogInfo("Key O");
      break;
    case Keys::P:
      _log->LogInfo("Key P");
      break;
    case Keys::Q:
      _log->LogInfo("Key Q");
      break;
    case Keys::R:
      _log->LogInfo("Key R");
      break;
    case Keys::S:
      _log->LogInfo("Key S");
      break;
    case Keys::T:
      _log->LogInfo("Key T");
      break;
    case Keys::U:
      _log->LogInfo("Key U");
      break;
    case Keys::V:
      _log->LogInfo("Key V");
      break;
    case Keys::W:
      _log->LogInfo("Key W");
      break;
    case Keys::X:
      _log->LogInfo("Key X");
      break;
    case Keys::Y:
      _log->LogInfo("Key Y");
      break;
    case Keys::Z:
      _log->LogInfo("Key Z");
      break;

    case Keys::Zero:
      _log->LogInfo("Key Zero");
      break;
    case Keys::One:
      _log->LogInfo("Key One");
      break;
    case Keys::Two:
      _log->LogInfo("Key Two");
      break;
    case Keys::Three:
      _log->LogInfo("Key Three");
      break;
    case Keys::Four:
      _log->LogInfo("Key Four");
      break;
    case Keys::Five:
      _log->LogInfo("Key Five");
      break;
    case Keys::Six:
      _log->LogInfo("Key Six");
      break;
    case Keys::Seven:
      _log->LogInfo("Key Seven");
      break;
    case Keys::Eight:
      _log->LogInfo("Key Eight");
      break;
    case Keys::Nine:
      _log->LogInfo("Key Nine");
      break;

    case Keys::NumZero:
      _log->LogInfo("Key NumZero");
      break;
    case Keys::NumOne:
      _log->LogInfo("Key NumOne");
      break;
    case Keys::NumTwo:
      _log->LogInfo("Key NumTwo");
      break;
    case Keys::NumThree:
      _log->LogInfo("Key NumThree");
      break;
    case Keys::NumFour:
      _log->LogInfo("Key NumFour");
      break;
    case Keys::NumFive:
      _log->LogInfo("Key NumFive");
      break;
    case Keys::NumSix:
      _log->LogInfo("Key NumSix");
      break;
    case Keys::NumSeven:
      _log->LogInfo("Key NumSeven");
      break;
    case Keys::NumEight:
      _log->LogInfo("Key NumEight");
      break;
    case Keys::NumNine:
      _log->LogInfo("Key NumNine");
      break;

    case Keys::FuncOne:
      _log->LogInfo("Key FuncOne");
      break;
    case Keys::FuncTwo:
      _log->LogInfo("Key FuncTwo");
      break;
    case Keys::FuncThree:
      _log->LogInfo("Key FuncThree");
      break;
    case Keys::FuncFour:
      _log->LogInfo("Key FuncFour");
      break;
    case Keys::FuncFive:
      _log->LogInfo("Key FuncFive");
      break;
    case Keys::FuncSix:
      _log->LogInfo("Key FuncSix");
      break;
    case Keys::FuncSeven:
      _log->LogInfo("Key FuncSeven");
      break;
    case Keys::FuncEight:
      _log->LogInfo("Key FuncEight");
      break;
    case Keys::FuncNine:
      _log->LogInfo("Key FuncNine");
      break;
    case Keys::FuncTen:
      _log->LogInfo("Key FuncTen");
      break;
    case Keys::FuncEleven:
      _log->LogInfo("Key FuncEleven");
      break;
    case Keys::FuncTwelve:
      _log->LogInfo("Key FuncTwelve");
      break;

    case Keys::Tab:
      _log->LogInfo("Key Tab");
      break;
    case Keys::Grave:
      _log->LogInfo("Key Grave");
      break;
    case Keys::Comma:
      _log->LogInfo("Key Comma");
      break;
    case Keys::Period:
      _log->LogInfo("Key Period");
      break;
    case Keys::SemiColon:
      _log->LogInfo("Key SemiColon");
      break;
    case Keys::Apostrophe:
      _log->LogInfo("Key Apostrophe");
      break;
    case Keys::Minus:
      _log->LogInfo("Key Minus");
      break;
    case Keys::Equals:
      _log->LogInfo("Key Equals");
      break;
    case Keys::LeftBracket:
      _log->LogInfo("Key LeftBracket");
      break;
    case Keys::RightBracket:
      _log->LogInfo("Key RightBracket");
      break;
    case Keys::FrontSlash:
      _log->LogInfo("Key FrontSlash");
      break;
    case Keys::BackSlash:
      _log->LogInfo("Key BackSlash");
      break;
    case Keys::BackSpace:
      _log->LogInfo("Key BackSpace");
      break;

    case Keys::ScrollLock:
      _log->LogInfo("Key ScrollLock");
      break;
    case Keys::NumsLock:
      _log->LogInfo("Key NumsLock");
      break;
    case Keys::CapsLock:
      _log->LogInfo("Key CapsLock");
      break;

    case Keys::PrintScreen:
      _log->LogInfo("Key PrintScreen");
      break;
    case Keys::Pause:
      _log->LogInfo("Key Pause");
      break;
    case Keys::Insert:
      _log->LogInfo("Key Insert");
      break;
    case Keys::Delete:
      _log->LogInfo("Key Delete");
      break;
    case Keys::Home:
      _log->LogInfo("Key Home");
      break;
    case Keys::End:
      _log->LogInfo("Key End");
      break;
    case Keys::PageUp:
      _log->LogInfo("Key PageUp");
      break;
    case Keys::PageDown:
      _log->LogInfo("Key PageDown");
      break;

    case Keys::NumDivide:
      _log->LogInfo("Key NumDivide");
      break;
    case Keys::NumMultiply:
      _log->LogInfo("Key NumMultiply");
      break;
    case Keys::NumMinus:
      _log->LogInfo("Key NumMinus");
      break;
    case Keys::NumAdd:
      _log->LogInfo("Key NumAdd");
      break;
    case Keys::NumEnter:
      _log->LogInfo("Key NumEnter");
      break;
    case Keys::NumDot:
      _log->LogInfo("Key NumDot");
      break;

    case Keys::Space:
      _log->LogInfo("Key Space");
      break;
    case Keys::Enter:
      _log->LogInfo("Key Enter");
      break;
    case Keys::Shift:
      _log->LogInfo("Key Shift");
      break;
    case Keys::Control:
      _log->LogInfo("Key Control");
      break;
    case Keys::Alt:
      _log->LogInfo("Key Alt");
      break;
    case Keys::Super:
      _log->LogInfo("Key Super");
      break;
    case Keys::Fn:
      _log->LogInfo("Key Fn");
      break;
    case Keys::Escape:
      _log->LogInfo("Key Escape");
      break;
    case Keys::Context:
      _log->LogInfo("Key Context");
      break;

    case Keys::ArrowLeft:
      _log->LogInfo("Key ArrowLeft");
      break;
    case Keys::ArrowRight:
      _log->LogInfo("Key ArrowRight");
      break;
    case Keys::ArrowUp:
      _log->LogInfo("Key ArrowUp");
      break;
    case Keys::ArrowDown:
      _log->LogInfo("Key ArrowDown");
      break;
    default:
      _log->LogInfo("Unknown Key");
      break;
  }

}

void VectorEvents::LogMouse(enum Mouse mouse, i32 x, i32 y) const
{
  switch (mouse)
  {
    case Mouse::Move:
      _log->LogInfo("Move, (%d, %d)", x, y);
      break;
    case Mouse::LeftDown:
      _log->LogInfo("LeftDown, (%d, %d)", x, y);
      break;
    case Mouse::LeftUp:
      _log->LogInfo("LeftUp, (%d, %d)", x, y);
      break;
    case Mouse::RightDown:
      _log->LogInfo("RightDown, (%d, %d)", x, y);
      break;
    case Mouse::RightUp:
      _log->LogInfo("RightUp, (%d, %d)", x, y);
      break;
    case Mouse::MiddleDown:
      _log->LogInfo("MiddleDown, (%d, %d)", x, y);
      break;
    case Mouse::MiddleUp:
      _log->LogInfo("MiddleUp, (%d, %d)", x, y);
      break;
    case Mouse::ScrollUp:
      _log->LogInfo("ScrollUp, (%d, %d)", x, y);
      break;
    case Mouse::ScrollDown:
      _log->LogInfo("ScrollDown, (%d, %d)", x, y);
      break;
    case Mouse::ButtonFourDown:
      _log->LogInfo("ButtonFourDown, (%d, %d)", x, y);
      break;
    case Mouse::ButtonFourUp:
      _log->LogInfo("ButtonFourUp, (%d, %d)", x, y);
      break;
    case Mouse::ButtonFiveDown:
      _log->LogInfo("ButtonFiveDown, (%d, %d)", x, y);
      break;
    case Mouse::ButtonFiveUp:
      _log->LogInfo("ButtonFiveUp, (%d, %d)", x, y);
      break;
    default:
      _log->LogInfo("Unknown Mouse");
      break;
  }
}

void VectorEvents::LogWindowState(enum WindowState window_state, i32 l, i32 r) const
{
  switch (window_state)
  {
    case WindowState::AppQuit:
      _log->LogInfo("AppQuit");
      break;
    case WindowState::WindowResize:
      _log->LogInfo("WindowResize");
      break;
    case WindowState::WindowMinimize:
      _log->LogInfo("WindowMinimize");
      break;
    case WindowState::WindowMaximize:
      _log->LogInfo("WindowMaximize");
      break;
    default:
      _log->LogInfo("Unknown Window");
      break;
  }

}

} // namespace ERI