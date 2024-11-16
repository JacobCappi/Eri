#include "GameWorld.h"

int main(int argc, char **argv)
{
    auto engine = new GameWorld(100, 100, 800, 900, "ERI ENGINE");
    engine->MainLoop();
    delete engine;
}

