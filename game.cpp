#include "base/timer.h"
#include "base/with_mass.h"
#include "game.h"
#include "show.h"
#include "player.h"
#include "enemies.h"
#include "checkpoints.h"
#include "fruits.h"
#include "box.h"
#include "traps.h"

Game::Game(): App("Tiles Adventure", 500, 400),
    indicateur("AngryPig", 0, 0, 0, "fonts/emulogic", NULL, 10, 5, 30),
    bg_timer(10)
{
    cur_enemy = cur_item = cur_trap = 0;
    focus = ENEMY;
    cursor = NULL;
    bg = new Background;
    sprites.add(new Player(&map, 60, 20, keys, "Ninja Frog"));
    sprites.add(new Fps(Fps::DEFAULT, "fonts/Supercell-Magic_5"));
    headers[ITEM] = new Text("Current item : ", 255, 255, 255, "fonts/Supercell-Magic_5",
                           NULL, 15, 5, indicateur.get_bottom());
    headers[ENEMY] = new Text("Current enemy : ", 255, 255, 255, "fonts/Supercell-Magic_5",
                           NULL, 15, 5, 5);
    headers[TRAP] = new Text("Current trap : ", 255, 255, 255, "fonts/Supercell-Magic_5",
                           NULL, 15, 5, 80);
    for (int i=0; i<3; ++i)
        sprites.add(headers[i]);
    Map::camera.x = width*.5 - 100;
    Map::camera.y = height*.5 + 10;
    Map::camera.w = 200;
    Map::camera.h = 120;
}

void Game::manage_events()
{
    if (keys[SDLK_KP8])
        Map::camera.y -= 2;
    else if (keys[SDLK_KP2])
        Map::camera.y += 2;
    if (keys[SDLK_KP4])
        Map::camera.x -= 2;
    else if (keys[SDLK_KP6])
        Map::camera.x += 2;
}

void Game::update_events()
{
    App::update_events();
    if (event.type == SDL_MOUSEMOTION)
    {
        if (cursor)
            cursor->set_position(event.motion.x, event.motion.y);
    }
    else if (event.type == SDL_MOUSEBUTTONUP)
    {
        SDL_Color color = {0, 0, 0};
        std::string traps_name[] = {"Arrow", "Block", "Falling Platform",
                                    "Fan", "Fire", "Platform",
                                    "Rock Head", "Saw", "Spike Head",
                                    "Spiked Ball", "Spikes", "Trampoline"
                                    };
        std::string items_name[] = {"Box1", "Box2", "Box3",
                                    "Checkpoint", "End", "Start",
                                    "Apple", "Bananas", "Cherries",
                                    "Kiwi", "Melon", "Orange",
                                    "Pineapple", "Strawberry"
                                    };
        std::string enemies_name[] = {"AngryPig", "Bunny", "Chicken",
                                      "Chameleon", "Duck", "Mushroom",
                                      "Trunk", "Plant", "Rino",
                                      "Turtle", "Skull"
                                      };
        // Item, Enemy, Trap
        int nbr[3] = {14, 11, 12};
        int x(event.button.x), y(event.button.y);
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            if (headers[ITEM]->collide_with(x, y))
            {
                focus = ITEM;
                indicateur.set(color, items_name[cur_item]);
                delete cursor;
                cursor = create_item(items_name[cur_item], true);
            }
            else if (headers[ENEMY]->collide_with(x, y))
            {
                focus = ENEMY;
                indicateur.set(color, enemies_name[cur_enemy]);
                delete cursor;
                cursor = NULL;
            }
            else if (headers[TRAP]->collide_with(x, y))
            {
                focus = TRAP;
                indicateur.set(color, traps_name[cur_trap]);
                delete cursor;
                cursor = create_trap(traps_name[cur_trap], true);
            }
            else
                switch (focus)
                {
                case ITEM:
                    if (cursor)
                        map.add_item(create_item(cursor->get_type()));
                    break;
                case ENEMY:
                    map.add_enemies(create_enemy(enemies_name[cur_enemy]));
                    break;
                case TRAP:
                    if (cursor)
                        map.add_traps(create_trap(cursor->get_type()));
                    break;
                default: ;
                }
        }

        else if (event.button.button == SDL_BUTTON_RIGHT)
            map.delete_sprite_at(x, y);

        else if (event.button.button == SDL_BUTTON_WHEELUP)
            switch (focus)
            {
            case ITEM:
                cur_item = (cur_item+1)%nbr[ITEM];
                indicateur.set(color, items_name[cur_item]);
                delete cursor;
                cursor = create_item(items_name[cur_item], true);
                break;
            case ENEMY:
                cur_enemy = (cur_enemy+1)%nbr[ENEMY];
                indicateur.set(color, enemies_name[cur_enemy]);
                delete cursor;
                cursor = NULL;
                break;
            case TRAP:
                cur_trap = (cur_trap+1)%nbr[TRAP];
                indicateur.set(color, traps_name[cur_trap]);
                delete cursor;
                cursor = create_trap(traps_name[cur_trap], true);
                break;
            default: ;
            }
        else if (event.button.button == SDL_BUTTON_WHEELDOWN)
            switch (focus)
            {
            case ITEM:
                cur_item--;
                if (cur_item<0)
                    cur_item = nbr[ITEM]-1;
                indicateur.set(color, items_name[cur_item]);
                delete cursor;
                cursor = create_item(items_name[cur_item], true);
                break;
            case ENEMY:
                cur_enemy--;
                if (cur_enemy<0)
                    cur_enemy = nbr[ENEMY]-1;
                indicateur.set(color, enemies_name[cur_enemy]);
                delete cursor;
                cursor = NULL;
                break;
            case TRAP:
                cur_trap--;
                if (cur_trap<0)
                    cur_trap = nbr[TRAP]-1;
                indicateur.set(color, traps_name[cur_trap]);
                delete cursor;
                cursor = create_trap(traps_name[cur_trap], true);
                break;
            default: ;
            }
        indicateur.set_position(5, headers[focus]->get_bottom());
    }
    else if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.sym == SDLK_SPACE)
            pause();
        else if (event.key.keysym.sym == SDLK_b)
            With_mass::showBoundingBox = !With_mass::showBoundingBox;
    }
}

void Game::draw()
{
    bg->draw(screen);
    // rectangleColor(screen, camera.x, camera.y, camera.x+camera.w, camera.y+camera.h, 0xff);
    map.draw(screen);
    sprites.draw(screen);
    indicateur.draw(screen);
    if (cursor) cursor->draw(screen);
    SDL_Flip(screen);
}

void Game::update()
{
    if (paused)
        return;
    sprites.update();
    if (Show::instance)
        return;
    bg->update();
    map.update();
    map.center_on(sprites[0], Map::camera);
    if (cursor) cursor->update();
    if (bg_timer.out())
    {
        bg_timer.restart();
        delete bg;
        bg = new Background;
    }
}

Sprite* Game::create_enemy(const std::string& name)
{
    int x(event.button.x+map.get_xshift()),
        y(event.button.y+map.get_yshift());
    if (name == "AngryPig")
        return new AngryPig(&map, x, y);
    if (name == "Bunny")
        return new Bunny(&map, x, y);
    if (name == "Chicken")
        return new Chicken(&map, x, y);
    if (name == "Chameleon")
        return new Chameleon(&map, x, y, sprites[0]);
    if (name == "Duck")
        return new Duck(&map, x, y);
    if (name == "Mushroom")
        return new Mushroom(&map, x, y);
    if (name == "Trunk")
        return new Trunk(&map, x, y, sprites[0], &map.bullets);
    if (name == "Plant")
        return new Plant(&map, x, y, sprites[0], &map.bullets);
    if (name == "Rino")
        return new Rino(&map, x, y, sprites[0]);
    if (name == "Turtle")
        return new Turtle(&map, x, y);
    if (name == "Skull")
        return new Skull(&map, x, y, sprites[0]);
    return NULL;
}

Sprite* Game::create_item(const std::string& name, bool icon)
{
    SDL_Rect* viewport(icon?NULL:map.get_viewport());
    Map* m(icon?NULL:&map);
    int x(event.button.x+map.get_xshift()),
        y(event.button.y+map.get_yshift());
    if (name == "Checkpoint")
        return new Checkpoint(x, y, viewport);
    if (name == "End")
        return new End(x, y, viewport);
    if (name == "Start")
        return new Start(x, y, viewport);
    if (name == "Box1")
        return new Box(m, x, y, 1);
    if (name == "Box2")
        return new Box(m, x, y, 2);
    if (name == "Box3")
        return new Box(m, x, y, 3);
    return new Fruit(x, y, m, name);
}

Sprite* Game::create_trap(const std::string& name, bool icon)
{
    SDL_Rect* viewport(icon?NULL:map.get_viewport());
    Map* m(icon?NULL:&map);
    int x(event.button.x+map.get_xshift()),
        y(event.button.y+map.get_yshift());
    if (name == "Arrow")
        return new Arrow(viewport, x, y);
    return new Falling_platform(viewport, x, y);
}
