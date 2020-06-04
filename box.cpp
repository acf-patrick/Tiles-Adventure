#include <sstream>
#include "box.h"

Box::Box(Map* m, int _x, int _y, int nbr):
    With_mass(m, _x, _y), numero(nbr),
    cur_image(0), state("Idle")
{
    hp = 10+numero*10;
    type.push_back("Boxes");
    std::stringstream t;
    t << "Box" << numero;
    type.push_back(t.str());
    x = _x;
    y = _y;
    rect.w = 28;
    rect.h = 24;
    std::stringstream oss;
    oss << "images/Items/Boxes/Box" << nbr;
    std::string img_name(oss.str());
    img_name += "/Hit (28x24).png";
    images["Hit"] = IMG_Load(img_name.c_str());
    img_name = oss.str() + "/Idle.png";
    image = images["Idle"] = IMG_Load(img_name.c_str());
}

Box::~Box()
{
    for (int i=0; i<2; ++i)
        for (std::map<std::string, SDL_Surface*>::iterator it=images.begin(); it!=images.end(); ++it)
        {
            SDL_FreeSurface(it->second);
            images[it->first] = NULL;
        }
    image = NULL;
}

void Box::update()
{
    image = images[state];
    if (timer.get_elapsed_ms() >= 75)
    {
        cur_image++;
        timer.restart();
    }
    if (cur_image>=image->w/rect.w and state == "Hit")
    {
        cur_image = 0;
        state = "Idle";
    }
    cur_image %= image->w/rect.w;
}

void Box::draw(SDL_Surface* screen)
{
    rect.x = cur_image*rect.w;
    rect.y = 0;
    int xs((!m_map)?0:m_map->get_xshift()), ys((!m_map)?0:m_map->get_yshift());
    SDL_Rect pos = { Sint16(x-xs), Sint16(y-ys) };
    SDL_BlitSurface(image, &rect, screen, &pos);
}

void Box::bump(const std::string& flag)
{
    if (state == "Idle")
    {
        cur_image = 0;
        state = "Hit";
        float impulse(0);
        std::stringstream iss(flag);
        iss >> impulse;
        hp -= impulse;
        if (hp <= 0)
        {
            state = "Break";
            for (int i=0; i<4; ++i)
                groups[0]->add(new BoxPiece(m_map, get_centerx(), get_centery(), numero, i));
            kill();
        }
    }
}
