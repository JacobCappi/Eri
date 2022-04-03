#include <IGame.h>
#include <string>


class testGame : public ERI::IGame
{

private:
    std::string game_name;
    i16 starting_x;
    i16 starting_y;
    i16 starting_w;
    i16 starting_h;

public:
    testGame();
    ~testGame() = default;

public:
    i16 get_init_x(void);
    i16 get_init_y(void);
    i16 get_init_w(void);
    i16 get_init_h(void);
    std::string get_init_name(void);

    b8 init_game(std::string name, i16 x, i16 y, i16 w, i16 h);

    b8 init(void);
    b8 update(f32 delta_time);
    b8 render(f32 delta_time);
    b8 on_resize(u32 w, u32 h);

};