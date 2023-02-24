#include "GameWorld.h"

int main(int argc, char **argv)
{
    auto engine = new GameWorld();
    engine->MainLoop();
    delete engine;
}

