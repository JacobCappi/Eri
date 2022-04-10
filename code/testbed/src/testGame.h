#include <IGame.h>
#include <string>


class testGame : public ERI::IGame
{

private:
//TODO: string lib refactor
    char *game_name = "Test Eri Engine";
    i16 starting_x;
    i16 starting_y;
    i16 starting_w;
    i16 starting_h;

public:
    testGame() = default;
    ~testGame() = default;

public:
    i16 get_init_x(void);
    i16 get_init_y(void);
    i16 get_init_w(void);
    i16 get_init_h(void);
    b8 get_init_name(char *buff, u16 size);

    b8 init_size(i16 x, i16 y, i16 w, i16 h);

    b8 init(void);
    b8 update(f32 delta_time);
    b8 render(f32 delta_time);
    b8 on_resize(u32 w, u32 h);

};