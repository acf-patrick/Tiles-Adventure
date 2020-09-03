#include <SDL/SDL_image.h>
#include <ctime>
#include "base/app.h"
#include "background.h"

Background::Background()
{
    std::string name[7] = {"Blue", "Brown", "Gray", "Green", "Pink", "Purple", "Yellow"};
    std::string n("images/Background/");
    n += name[rand()%7] + ".png";
    image = IMG_Load(n.c_str());
    if (!image)
    {
        std::cerr << IMG_GetError();
        exit(EXIT_FAILURE);
    }
    int i, window_height;
    App::instance->window_size(NULL, &window_height);
    for (i=0; i<window_height+image->h; i+=image->h)
        ys.push_back(i);
}

void Background::update()
{
    for (int i=0; i<(int)ys.size(); ++i)
    {
        if (ys[i]+image->h <= 0)
            ys[i] = ys[i?(i-1):ys.size()-1]+image->h;
        ys[i] -= 2;
    }
}

void Background::draw(SDL_Surface* screen)
{
    int window_width;
    App::instance->window_size(&window_width, NULL);
    for (int i=0; i<window_width; i+=image->w)
    {
        for (int j=0; j<(int)ys.size(); ++j)
        {
            rect.x = i;
            rect.y = ys[j];
            SDL_BlitSurface(image, NULL, screen, &rect);
        }
    }
}
