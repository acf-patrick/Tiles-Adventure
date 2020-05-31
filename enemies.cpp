#include "base/func_tool.h"
#include <SDL_image.h>
#include <algorithm>
#include <SDL_gfxPrimitives.h>
#include <ctime>
#include <cmath>
#include "enemies.h"
#include "bullet.h"

Chicken::Chicken(Map* m, int _x, int _y):
    Enemy(m, _x, _y, "Chicken", 32, 34)
{}

void Chicken::update()
{
    update_image();
    if (erase())
        return;
    apply_gravity();
    move(x_vel, y_vel);
    if (state == "Run")
    {
        int s(direction?1:-1);
        x_vel = s*2;
        x += 2*s;
        int _x = direction?get_right():(x);
        if (!m_map->get_tile_nbr(_x, get_bottom()) or m_map->collision_with(this))
        {
            x_vel *= -1;
            direction = !direction;
        }
        x -= 2*s;
        stop_falling();
    }
    else
        x_vel = 0;
    wait();
    cur_image %= (image->w/rect.w);
}

Bunny::Bunny(Map* m, int _x, int _y):
    Enemy(m, _x, _y, "Bunny", 34, 44)
{
    gravity *= .5;
    std::vector<std::string> key;
    key.push_back("Jump");
    key.push_back("Fall");
    load_images(key);
}

void Bunny::update()
{
    update_image();
    if (erase())
        return;
    if (state != "Idle")
    {
        if (y_vel>0)
            state = "Fall";
        else if (y_vel<0)
            state = "Jump";
        else
            state = "Run";
    }
    else
        x_vel = 0;
    if (state == "Run")
        x_vel = direction?2:-2;
    apply_gravity();
    move(x_vel, y_vel);

    int s(x_vel);
    x += 2*s;
    if (m_map->collision_with(this))
    {
        if (rand()%2)
        {
            x_vel *= -1;
            direction = !direction;
        }
        else
            y_vel = -7;
    }
    x -= 2*s;
    y += 2;
    if (y_vel >= 0)
        if (m_map->collision_with(this))
        {
            int s_(direction?1:-1);
            x += s_*2;
            if (!m_map->get_tile_nbr(direction?get_right():(x-2), get_bottom()))
            {
                y_vel = -7;
                x_vel = s_*2;
            }
            else
                y_vel = 0;
            x -= s_*2;
        }
    y -= 4;
    if (m_map->collision_with(this))
        y_vel = 2;
    y += 2;

    wait();
    check_down = y_vel>0;
    cur_image %= (image->w/rect.w);
}

AngryPig::AngryPig(Map* m, int _x, int _y):
    Enemy(m, _x, _y, "AngryPig", 36, 30)
{
    load_images(std::vector<std::string> (1, "Walk"));
}

bool AngryPig::collide_with(Sprite* sprite)
{
    if (sprite)
        if ((state == "Walk" or state == "Idle") and sprite->is("Player"))
            return false;
    return Sprite::collide_with(sprite);
}

void AngryPig::bump(const std::string& flag)
{
    if (flag == "shotted")
        state = (state == "Run")?"Walk":"Run";
    else
        Enemy::bump();
}

void AngryPig::update()
{
    image = images[direction][state];
    if (timer.get_elapsed_ms() >= ((state == "Run")?30:50))
    {
        cur_image = (cur_image+1);
        timer.restart();
    }
    if (erase())
        return;
    apply_gravity();
    move(x_vel, y_vel);
    if (state == "Run" or state == "Walk")
    {
        x_vel = (state == "Run")?3:1;
        int s(direction?1:-1);
        x_vel *= s;
        x += 2*s;
        int _x = direction?get_right():x;
        if ((state == "Walk" and !m_map->get_tile_nbr(_x, get_bottom())) or
                m_map->collision_with(this))
        {
            x_vel *= -1;
            direction = !direction;
        }
        x -= 2*s;
        stop_falling();
    }
    else
        x_vel = 0;

    if (state_timer.get_elapsed_s() >= delay)
    {
        state = (state == "Idle")?((rand()%2)?"Run":"Walk"):"Idle";
        if (state == "Idle")
            delay = randint(1, 3);
        else
            delay = randint(5, 10);
        state_timer.restart();
    }
    cur_image %= (image->w/rect.w);
}

Chameleon::Chameleon(Map* m, int _x, int _y, Sprite* cible):
    Enemy(m, _x, _y, "Chameleon", 84, 38), target(cible)
{
    load_images(std::vector<std::string>(1, "Attack"));
    img_rect.w = rect.w;
    img_rect.h = rect.h;
    rect.w = 38;
}

SDL_Surface* Chameleon::get_surface()
{
    rect.w = img_rect.w;
    SDL_Surface* ret(Enemy::get_surface());
    rect.w = 38;
    return ret;
}

void Chameleon::draw(SDL_Surface* screen)
{
    show_boundingBox(screen);
    img_rect.x = cur_image*img_rect.w;
    img_rect.y = 0;
    SDL_Rect pos = {Sint16(x - m_map->get_xshift()), Sint16(y - m_map->get_yshift())};
    if (!direction)
        pos.x -= img_rect.w - rect.w;
    SDL_BlitSurface(image, &img_rect, screen, &pos);
}

void Chameleon::update()
{
    update_image();
    if (erase())
        return;

    SDL_Rect field;
    field.x = Sint16(x);
    if (!direction)
        field.x -= (img_rect.w - rect.w);
    field.y = Sint16(y+0.25*rect.h);
    field.w = img_rect.w;
    field.h = 0.5*img_rect.h;
    if (target->collide_with(field))
    {
        if (state != "Attack")
            cur_image = 0;
        state = "Attack";
    }
    else
        if (state == "Attack")
            state = "Run";
    if (state_timer.get_elapsed_s() >= delay)
    {
        state = (state == "Idle")?"Run":"Idle";
        if (state == "Run")
            delay = randint(5, 10);
        else
            delay = randint(1, 3);
        state_timer.restart();
    }

    apply_gravity();
    move(x_vel, y_vel);

    if (state == "Run")
    {
        int s(direction?1:-1);
        x_vel = s*2;
        x += 2*s;
        int _x = direction?get_right():(x);
        bool bound(!m_map->get_tile_nbr(_x, get_bottom()));
        bool collision(m_map->collision_with(this));
        if (bound or collision)
        {
            if (collision)
                std::cout << "here\n";
            x_vel *= -1;
            direction = !direction;
            image = images[direction][state];
        }
        x -= 2*s;
        stop_falling();
    }
    else
        x_vel = 0;

    cur_image %= (image->w/img_rect.w);
}

bool Chameleon::collide_with(Sprite* sprite)
{
    if (!sprite)
        return NULL;
    if (sprite->is("Player"))
    {
        SDL_Surface *s1(sprite->get_surface()), *s2(get_surface());
        int _x = x;
        if (!direction)
            _x -= img_rect.w - rect.w;
        bool ret = images_collide(_x-m_map->get_xshift(), y-m_map->get_yshift(), s2,
                            sprite->get_x()-m_map->get_xshift(), sprite->get_y()-m_map->get_yshift(), s1);
        SDL_FreeSurface(s1);
        SDL_FreeSurface(s2);
        return ret;
    }
    return Sprite::collide_with(sprite);
}

Duck::Duck(Map* m, int _x, int _y):
    Enemy(m, _x, _y, "Duck", 36, 36)
{
    std::vector<std::string> key;
    key.push_back("Jump Anticipation");
    key.push_back("Fall");
    key.push_back("Jump");
    state = "Fall";
    gravity *= 0.75;
    load_images(key);
}

void Duck::update()
{
    update_image();
    if (erase())
        return;

    if (state == "Idle" or state == "Jump Anticipation")
        x_vel = 0;
    else
    {
        if (y_vel<0)
            state = "Jump";
        else if (y_vel>0)
            state = "Fall";
        x_vel = 4*(direction?1:-1);
    }

    apply_gravity();
    move(x_vel, y_vel);

    if (state == "Idle")
    {
        if (state_timer.get_elapsed_s() >= delay)
        {
            state = "Jump Anticipation";
            delay = randint(1, 5);
            state_timer.restart();
        }
        cur_image %= (image->w/rect.w);
        return;
    }

    y += 2;
    if (m_map->collision_with(this))
        state = "Jump Anticipation";
    y -= 2;
    int s(sgn(x_vel));
    x += 2*s;
    if (m_map->collision_with(this))
    {
        direction = !direction;
        state = "Idle";
        state_timer.restart();
    }
    x -= 2*s;

    if (state == "Jump Anticipation" and
            cur_image >= images[direction][state]->w/rect.w)
    {
        state = "Jump";
        y_vel = -7;
    }
    cur_image %= (image->w/rect.w);
}

Mushroom::Mushroom(Map* m, int _x, int _y):
    Enemy(m, _x, _y, "Mushroom", 32, 32)
{}

void Mushroom::update()
{
    update_image();
    if (erase())
        return;
    apply_gravity();
    move(x_vel, y_vel);
    if (state == "Run")
    {
        int s(direction?1:-1);
        x_vel = s;
        x += 2*s;
        if (m_map->collision_with(this))
        {
            x_vel *= -1;
            direction = !direction;
        }
        x -= 2*s;
        stop_falling();
    }
    else
        x_vel = 0;
    wait();
    cur_image %= (image->w/rect.w);
}

Trunk::Trunk(Map* m, int _x, int _y, Sprite* cible, Group* bullets_group):
    Shot("Trunk", m, _x, _y, 64, 32, cible, bullets_group)
{
    load_images(std::vector<std::string>(1, "Attack"));
}

void Trunk::update()
{
    update_image();
    if (erase())
        return;
    apply_gravity();
    move(x_vel, y_vel);

    SDL_Rect field;
    field.w = 50;
    field.h = rect.h;
    field.x = Sint16(direction?get_right():(x-field.w));
    field.y = Sint16(y);
    if (target->collide_with(field))
        state = "Attack";
    else if (state != "Idle")
        state = "Run";

    if (state != "Idle")
    {
        int s(direction?1:-1);
        if (state == "Run")
            x_vel = s*2;
        else
        {
            if (cur_image == (direction?3:7))
                fire();
            x_vel = s;
        }
        x += 2*s;
        int _x = direction?get_right():(x);
        if (!m_map->get_tile_nbr(_x, get_bottom()) or m_map->collision_with(this))
        {
            x_vel *= -1;
            state = "Run";
            direction = !direction;
            image = images[direction][state];
        }
        x -= 2*s;
        stop_falling();
    }
    else
        x_vel = 0;

    wait();
    cur_image %= (image->w/rect.w);

}

Plant::Plant(Map* m, int _x, int _y, Sprite* cible, Group* bullets_group):
    Shot("Plant", m, _x, _y, 44, 42, cible, bullets_group)
{}

void Plant::update()
{
    direction = (x < target->get_x());
    update_image();
    if (erase())
        return;
    apply_gravity();
    if (y_vel)
        move(x_vel, y_vel);
    SDL_Rect field;
    field.w = 50;
    field.h = rect.h;
    field.x = Sint16(direction?get_right():(x-field.w));
    field.y = Sint16(y);
    state = (target->collide_with(field))?"Attack":"Idle";
    if (y_vel)
        stop_falling();
    if (state == "Attack" and cur_image == (direction?3:4))
        fire();
    cur_image %= (image->w/rect.w);
}

Rino::Rino(Map* m, int _x, int _y, Sprite* cible):
    Enemy(m, _x, _y, "Rino", 52, 34),
    charge(0), target(cible)
{
    load_images(std::vector<std::string>(1, "Hit Wall"));
}

void Rino::update()
{
    image = images[direction][state];
    int animation_delay(0);
    if (state == "Hit Wall")
        animation_delay = 30;
    else
        animation_delay = 100-std::abs(x_vel)*10;

    if (timer.get_elapsed_ms() >= animation_delay)
    {
        cur_image = (cur_image+1);
        timer.restart();
    }
    if (erase())
        return;

    if (charge)
        direction = (x < target->get_x());
    apply_gravity();
    move(x_vel, y_vel);
    stop_falling();

    if (state == "Hit Wall")
    {
        if (cur_image >= image->w/rect.w)
            state = "Run";
        else
            return;
    }

    SDL_Rect field;
    field.w = 60;
    field.h = rect.h;
    field.x = Sint16(direction?get_right():(x-field.w));
    field.y = Sint16(y);
    if (target->collide_with(field))
    {
        charge = true;
        state = "Run";
    }

    if (state == "Run")
    {
        int s = direction?1:-1;
        if (charge)
            x_vel += s*((s!=sgn(x_vel))?0.2:0.15);
        else
            x_vel = 2*s;

        x += 2*s;
        bool collision(m_map->collision_with(this)),
             bound(!m_map->get_tile_nbr((direction?get_right():x), get_bottom()));
        if (collision or bound)
        {
            if (charge)
            {
                if (collision)
                {
                    cur_image = 0;
                    state = "Hit Wall";
                    charge = false;
                    x_vel = -8*s;
                    y_vel = -8;
                }
                else if (bound)
                    state = "Idle";
            }
            else
            {
                direction = !direction;
                x_vel *= -1;
            }
        }
        x -= 2*s;
    }

    if (state == "Idle")
    {
        charge = false;
        x_vel = 0;
    }

    if (state!="Hit Wall")
        if (!charge)
            wait();
    cur_image %= (image->w/rect.w);
}

Turtle::Turtle(Map* m, int _x, int _y):
    Enemy(m, _x, _y, "Turtle", 44, 26)
{
    std::vector<std::string> keys;
    keys.push_back("Idle 1"); // Spikes are out
    keys.push_back("Idle 2"); // Spikes are in
    keys.push_back("Spikes in");
    keys.push_back("Spikes out");
    load_images(keys);
    SDL_FreeSurface(images[1]["Spikes in"]);
    SDL_FreeSurface(images[1]["Spikes out"]);
    SDL_Surface* tmp_1 = images[0]["Spikes out"];
    SDL_Surface* tmp_2 = images[0]["Spikes in"];
    images[1]["Spikes in"] = flip(tmp_1, true, false);
    images[1]["Spikes out"] = flip(tmp_2, true, false);
    state = "Idle 2";
}

void Turtle::update()
{
    update_image();
    if (erase())
        return;
    if (cur_image == 7)
    {
        if (state == "Spikes in")
            state = "Idle 2";
        else if (state == "Spikes out")
            state = "Idle 1";
    }
    apply_gravity();
    move(x_vel, y_vel);
    if (state.find("Idle") != state.npos)
        x_vel = direction?1:-1;
    else
        x_vel = 0;
    stop_falling();

    int s(direction?1:-1);
    x += 2*s;
    if (m_map->collision_with(this) or
            !m_map->get_tile_nbr((direction?get_right():x), get_bottom()))
        direction = !direction;
    x -= 2*s;

    if (state_timer.get_elapsed_s() >= delay)
    {
        if (state == "Idle 1")
            state = "Spikes in";
        else if (state == "Idle 2")
            state = "Spikes out";
        delay = randint(1, 10);
        cur_image = 0;
        state_timer.restart();
    }
    cur_image %= (image->w/rect.w);
    check_up = (state != "Idle 2");
}

Skull::Skull(Map* m, int _x, int _y, Sprite* cible):
    Enemy(m, _x, _y, "Skull", 52, 54),
    charge(false), target(cible)
{
    std::vector<std::string> keys;
    keys.push_back("Hit Wall 1");
    keys.push_back("Hit Wall 2");
    keys.push_back("Idle 1");
    keys.push_back("Idle 2");
    load_images(keys);

    std::swap(images[0], images[1]);
    state = "Idle 2";
}

void Skull::update()
{
    particles.update();
    if (state == "Idle 1")
        if (particle_timer.get_elapsed_ms() >= 300)
        {
            particles.add(new Particle(direction?get_right():x, y, m_map->get_viewport()));
            particle_timer.restart();
        }
    image = images[direction][state];
    if (timer.get_elapsed_ms() >= 100-std::abs(x_vel)*10)
    {
        cur_image++;
        timer.restart();
    }

    if (state.find("Hit") != state.npos)
    {
        if (cur_image >= image->w/rect.w)
            kill();
        return;
    }
    if (charge)
        direction = (x < target->get_x());
    apply_gravity();
    move(x_vel, y_vel);
    stop_falling();

    field.w = 40;
    field.h = rect.h;
    field.x = Sint16(direction?get_right():(x-field.w));
    field.y = Sint16(y);
    if (target->collide_with(field))
        charge = true;

    {
        int s = direction?1:-1;
        if (charge)
            x_vel += s*((s!=sgn(x_vel))?0.2:0.15);
        else
            x_vel = (state == "Idle 2")?s:(2*s);

        x += 2*s;
        bool collision(m_map->collision_with(this)),
             bound(!m_map->get_tile_nbr((direction?get_right():x), get_bottom()));
        if (collision or bound)
        {
            if (charge)
            {
                if (collision)
                {
                    cur_image = 0;
                    state = (state == "Idle 1")?"Hit Wall 1":"Hit Wall 2";
                }
                else
                {
                    charge = false;
                    direction = !direction;
                    x_vel *= -1;
                }
            }
            else
            {
                direction = !direction;
                x_vel *= -1;
            }
        }
        x -= 2*s;
    }

    if (!charge)
        if (state_timer.get_elapsed_s() >= delay)
        {
            state = (state == "Idle 1")?"Idle 2":"Idle 1";
            delay = randint(5, 10);
            state_timer.restart();
        }
    cur_image %= (image->w/rect.w);
}

void Skull::draw(SDL_Surface* screen)
{
    Enemy::draw(screen);
    particles.draw(screen);
    /* int _x = field.x-m_map->get_xshift(), _y = field.y-m_map->get_yshift();
    rectangleColor(screen, _x, _y, _x+field.w, _y+field.h, 0xff); */
}
