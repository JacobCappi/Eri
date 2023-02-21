#pragma once


// I assume the app will pass all subsystems to Renderer?
// Factory order
//   Every subsystem needs logger in constructor  i.e. PlatformWindow(ILogger)
//   Factory will build it first and inject it in?
//   Factory's API should be 'buildPlatform()' returns IPlatform, passed into IRenderer(IPlatform, ILogger, IEventHandler, etc..)



// How does platform log?
