#pragma once

#include <App.h>

class GameWorld : public ERI::App
{
public:
    GameWorld(i32 x, i32 y, i32 w, i32 h, const char *name) :
    App(x, y, w, h, name) {}
    ~GameWorld() {}
};