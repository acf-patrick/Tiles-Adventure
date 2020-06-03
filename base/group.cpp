#include <algorithm>
#include "group.h"

std::vector<Sprite*> Group::buffer;

Group::~Group()
{
    for (int i=0; i<(int)sprite_list.size(); ++i)
    {
        buffer.erase(std::remove(buffer.begin(), buffer.end(), sprite_list[i]));
        delete sprite_list[i];
        sprite_list[i] = NULL;
    }
    sprite_list.clear();
}

void Group::clear_buffer()
{
    if (!buffer.empty())
    {
        for (int i=0; i<(int)buffer.size(); ++i)
        {
            delete buffer[i];
            buffer[i] = NULL;
        }
        buffer.clear();
    }
}

void Group::_add(Sprite* sprite)
{
    if (!sprite)
        return;
    if (sprite_list.end() == std::find(sprite_list.begin(), sprite_list.end(), sprite))
    {
        sprite_list.push_back(sprite);
        sprite->add(this);
    }
    if (buffer.end() == std::find(buffer.begin(), buffer.end(), sprite))
        buffer.push_back(sprite);
}
void Group::add(Sprite* sprite)
{
    _add(sprite);
}
void Group::add(std::vector<Sprite*> sprites)
{
    for (int i=0; i<(int)sprites.size(); ++i)
        _add(sprites[i]);
}

bool Group::empty() { return sprite_list.empty(); }

void Group::draw(SDL_Surface* screen)
{
    for (int i=0; i<(int)sprite_list.size(); ++i)
        sprite_list[i]->draw(screen);
}

bool Group::_has(Sprite* sprite)
{
    return (sprite_list.end() != std::find(sprite_list.begin(), sprite_list.end(), sprite));
}

bool Group::has(Sprite* sprite)
{
    return _has(sprite);
}

bool Group::has(std::vector<Sprite*> sprites)
{
    for (int i=0; i<(int)sprites.size(); ++i)
        if (!_has(sprites[i]))
            return false;
    return true;
}

void Group::_remove(Sprite* sprite)
{
    sprite_list.erase(std::remove(sprite_list.begin(), sprite_list.end(), sprite));
}

void Group::remove(Sprite* sprite)
{
    //if (!has(sprite)) return;
    sprite->groups.erase(std::remove(sprite->groups.begin(), sprite->groups.end(), this));
    _remove(sprite);
}

void Group::remove(std::vector<Sprite*> sprite)
{
    for (int i=0; i<(int)sprite.size(); ++i)
        _remove(sprite[i]);
}

std::vector<Sprite*> Group::sprites()
{
    return sprite_list;
}

Sprite* Group::get(int index)
{
    if (index >= (int)sprite_list.size() or !sprite_list.size())
        return NULL;
    return sprite_list[index];
}
Sprite* Group::operator[](int index)
{
    return get(index);
}

void Group::update()
{
    for (int i=0; i<(int)sprite_list.size(); ++i)
        sprite_list[i]->update();
}

std::vector<Sprite*> Group::sprites_colliding_with(Sprite* sprite, bool _kill)
{
    std::vector<Sprite*> ret;
    if (sprite)
    {
        Sprite* cur_sprite(NULL);

        for (int i=0; i<(int)sprite_list.size(); ++i)
        {
            cur_sprite = sprite_list[i];
            if (cur_sprite == sprite)
                continue;
            if (cur_sprite->collide_with(sprite) and
                sprite->collide_with(cur_sprite))
            {
                ret.push_back(cur_sprite);
                if (_kill)
                    this->remove(cur_sprite);
            }
        }
    }
    return ret;
}

Sprite* Group::first_sprite_colliding_with(Sprite* sprite)
{
    if (sprite)
    {
        for (int i=0; i<(int)sprite_list.size(); ++i)
            if (sprite != sprite_list[i])
                if (sprite_list[i]->collide_with(sprite) and
                    sprite->collide_with(sprite_list[i]))
                    return sprite_list[i];
    }
    return NULL;
}
Sprite* Group::first_sprite_colliding_with(SDL_Rect rect)
{
    Sprite s(rect);
    return first_sprite_colliding_with(&s);
}
Sprite* Group::first_sprite_colliding_with(int x, int y)
{
    SDL_Rect r = {Sint16(x), Sint16(y)};
    return first_sprite_colliding_with(r);
}

std::map< Sprite*, std::vector<Sprite*> > Group::collide_with(Group* group, bool _kill, bool __kill)
{
    int i, j;
    std::map< Sprite*, std::vector<Sprite*> > ret;
    std::vector<Sprite*> add_in_map;
    Sprite *s1(NULL), *s2(NULL);

    for (i=0; i<(int)sprite_list.size(); ++i)
    {
        s1 = sprite_list[i];
        for (j=0; j<(int)group->sprite_list.size(); ++i)
        {
            s2 = group->sprite_list[j];
            if (s1 == s2)
                continue;
            if (s1->collide_with(s2) and s2->collide_with(s1))
                add_in_map.push_back(s2);
            if (__kill)
                s2->kill();
            if (_kill)
                s1->kill();
        }
        if (!(add_in_map.empty()))
        {
            ret[s1] = add_in_map;
            add_in_map.clear();
        }
    }
    return ret;
}
