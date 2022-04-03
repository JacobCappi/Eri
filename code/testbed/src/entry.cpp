#include <entry.h>
#include "testGame.h"

//TODO: use my memory system
extern ERI::IGame* create_game(void)
{
    testGame *game_instance;
    game_instance = static_cast<testGame*>(malloc(sizeof(testGame)));

    return game_instance;
}