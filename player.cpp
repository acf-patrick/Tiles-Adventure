#include <cmath>
#include "player.h"
#include "base/func_tool.h"
#include "base/app.h"

Player::Player(Map* m, int _x, int _y, bool* k, const std::string& name) :
    With_mass(m, _x, _y), keys(k),
    state("Idle"), cur_image(0),
    double_jump(false), JUMP_GRAVITY(0.3),
    JUMP_IMPULSE(-7), MAX_FALL_SPEED(10)
{
    rect.w = rect.h = 32;
    type.push_back("Player");
    std::string key[7] = {"Double Jump", "Fall", "Hit", "Idle", "Jump", "Run", "Wall Jump"};
    for (int i=0; i<7; ++i)
    {
        std::string path ("images/Main Characters/");
        path += name + "/" + key[i] + " (32x32).png";
        images[1][key[i]] = IMG_Load(path.c_str());
        images[0][key[i]] = flip(images[1][key[i]], true, false);
    }
    show = new Show(x+rect.w*.5-m_map->get_xshift(), y+rect.h*.5-m_map->get_yshift());
}
Player::~Player()
{
    for (int i=0; i<2; ++i)
        for (std::map<std::string, SDL_Surface*>::iterator it=images[i].begin(); it!=images[i].end(); ++it)
        {
            SDL_FreeSurface(it->second);
            images[i][it->first] = NULL;
        }
    image = NULL;
}

void Player::update()
{
    if (show)
    {
        if (show->is_out())
        {
            if (show->desappear)
                return respawn();
                // kill();
            delete show;
            show = NULL;
            return;
        }
        show->update();
        return;
    }
    dust.update();
    image = images[direction][state];
    update_images();
    update_states();
    update_keys();
    apply_gravity();
    move(x_vel, y_vel);
    update_xvel();
    update_yvel();
}

void Player::draw(SDL_Surface* screen)
{
    show_boundingBox(screen);
    if (show)
    {
        show->draw(screen);
        return;
    }
    rect.x = cur_image*rect.w;
    rect.y = 0;
    SDL_Rect pos = {Sint16(x-m_map->get_xshift()), Sint16(y-m_map->get_yshift())};
    SDL_BlitSurface(image, &rect, screen, &pos);
    dust.draw(screen);
}

void Player::jump()
{
    gravity = JUMP_GRAVITY;
    y_vel = JUMP_IMPULSE;
    dust.add(new Dust(m_map->get_viewport(), get_centerx(), get_centery()+3, "Jump"));
}

void Player::update_images()
{
    int delay;
    if (state == "Run")
        delay = std::abs(x_vel)*25;
    else
        if (state == "Double Jump")
            delay = 50;
        else
            delay = 75;
    if (timer.get_elapsed_ms() >= delay)
    {
        cur_image++;
        timer.restart();
    }
}

void Player::update_states()
{
    if (int(x_vel) == 0)
        state = "Idle";
    else
        state = "Run";
    if (y_vel > 0)
        state = "Fall";
    else if (y_vel < 0)
        state = double_jump?"Double Jump":"Jump";
    cur_image %= (image->w/rect.w);
}

void Player::update_keys()
{
    if (keys[SDLK_RIGHT])
    {
        direction = RIGHT;
        x_vel = 2;
    }
    else if (keys[SDLK_LEFT])
    {
        direction = LEFT;
        x_vel = -2;
    }
    else
        x_vel = 0;
    if (keys[SDLK_q])
    {
        if (allow_jump)
        {
            allow_jump = false;
            y += 2;
            if (m_map->collision_with(this))
                jump();
            else
                if (!double_jump)
                {
                    double_jump = true;
                    jump();
                }
            y -= 2;
        }
    }
    else if (keys[SDLK_DOWN])
        gravity += 0.2;
    else
    {
        allow_jump = true;
        gravity = GRAVITY;
    }
}

void Player::update_xvel()
{
    int s(sgn(x_vel));
    x += 2*s;
    if (m_map->collision_with(this))
        x_vel = 0;
    x -= 2*s;
}
void Player::update_yvel()
{
    if (y_vel > MAX_FALL_SPEED)
        y_vel = MAX_FALL_SPEED;
    y += 2;
    if (m_map->collision_with(this))
    {
        double_jump = false;
        if (y_vel>8)
            dust.add(new Dust(m_map->get_viewport(), get_centerx(), get_centery()+3, "Fall"));
        y_vel = 0;
    }
    y -= 2;
    check_up = true;
    y -= 2;
    if (m_map->collision_with(this))
        y_vel = 0;
    y += 2;
    check_up = false;
    check_down = y_vel>0;
}

void Player::bump(const std::string& flag)
{
    if (flag.find("repulsion") != flag.npos)
    {
        double_jump = false;
        if (flag == "box repulsion")
        {
            gravity =  JUMP_GRAVITY;
            y_vel = -8;
        }
        else if (flag == "platform repulsion")
        {
            gravity =  JUMP_GRAVITY;
            y_vel = -4;
        }
        else if (flag == "arrow repulsion")
            y_vel = -15;
    }
    else
    {
        y_vel = -5;
        if (flag == "die")
            show = new Show(x+rect.w*.5-m_map->get_xshift(), y+rect.h*.5-m_map->get_yshift(), false);
    }
}

void Player::respawn()
{
    do
    {
        x = m_map->get_xshift() + randint(0, App::width);
        y = m_map->get_yshift() + randint(0, 0.5*App::height);
    } while(m_map->collision_with(this));
    m_map->center_on(this);
    show = new Show(x+rect.w*.5-m_map->get_xshift(), y+rect.h*.5-m_map->get_yshift());
}

float *Player::get_impulse()
{
    float* ret(Sprite::get_impulse());
    ret[0] = x_vel;
    ret[1] = y_vel;
    return ret;
}

SDL_Surface* Player::get_surface()
{
    SDL_Surface* s1 = SDL_CreateRGBSurface(SDL_HWSURFACE, rect.w, rect.h, 32,
                        image->format->Rmask, image->format->Gmask, image->format->Bmask, image->format->Amask);
    SDL_FillRect(s1, NULL, SDL_MapRGB(s1->format, 100, 100, 100));
    rect.x = cur_image*rect.w;
    rect.y = 0;
    SDL_BlitSurface(image, &rect, s1, 0);
    return s1;
}

bool Player::collide_with(Sprite* sprite)
{
    if (sprite->is("Enemy"))
    {
        // laisse la classe Chameleon gêrer la collision
        if (sprite->is("Chameleon"))
            return true;

        SDL_Surface *s1 = get_surface(), *s2 = sprite->get_surface();
        bool ret = images_collide(x-m_map->get_xshift(), y-m_map->get_yshift(), s1,
                            sprite->get_x()-m_map->get_xshift(), sprite->get_y()-m_map->get_yshift(), s2);
        SDL_FreeSurface(s1);
        SDL_FreeSurface(s2);
        return ret;
    }
    return Sprite::collide_with(sprite);
}
