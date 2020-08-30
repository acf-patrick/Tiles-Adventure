#include "base/with_mass.h"
#include "base/creator.h"
#include "player.h"
#include "game.h"
#include "show.h"
#include <SDL_gfxPrimitives.h>
#include <SDL_image.h>

Game::Game(): App("Tiles Adventure", 480, 360),
    bg_timer(10)
{
    IMG_Init(IMG_INIT_PNG);
    sprites.add(new Player(&map, 60, 50, keys, "Virtual Guy"));

    bg = new Background;
    sprites.add(new Fps(Fps::DEFAULT, "fonts/Supercell-Magic_5"));
    Map::camera.x = screen->w*.5 - 100;
    Map::camera.y = screen->h*.5 + 10;
    Map::camera.w = 200;
    Map::camera.h = 120;

    ObjectCreator::addToParameters(sprites[0], "Player");
    map.load_objects();
}
Game::~Game()
{
    delete bg; bg = NULL;
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

    if (event.type == SDL_KEYDOWN)
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_SPACE:
            pause();
            break;
        case SDLK_b:
            With_mass::showBoundingBox = !With_mass::showBoundingBox;
            break;
        default: ;
        }
    }
}

void Game::draw()
{
    bg->draw(screen);
    // rectangleColor(screen, camera.x, camera.y, camera.x+camera.w, camera.y+camera.h, 0xff);
    map.draw(screen);
    sprites.draw(screen);

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
    map.center_on(sprites[0] , Map::camera);
    if (bg_timer.out())
    {
        bg_timer.restart();
        change_background();
    }
}
void Game::change_background()
{
    delete bg;
    bg = new Background;
}
