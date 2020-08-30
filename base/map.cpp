#include <SDL_image.h>
#include <tmx.h>
#include "app.h"
#include "group.h"
#include "creator.h"
#include "map.h"
#include "func_tool.h"

SDL_Rect Map::camera = {0};

Map::Map(const std::string& map_file_name, SDL_Rect* viewport_rect):
    viewport(viewport_rect)
{
    std::cout << "Loading the game map... " << std::endl;

    std::cout << "Loading ressources... " << std::endl;
    if (!viewport)
    {
        viewport = new SDL_Rect;
        viewport->x = viewport->y = 0;
        int w, h;
        App::instance->window_size(&w, &h);
        viewport->w = w;
        viewport->h = h;
    }

    // set callback functions
    tmx_img_load_func = (void* (*)(const char*))IMG_Load;
    tmx_img_free_func = (void (*)(void*))SDL_FreeSurface;

    m_map = tmx_load(map_file_name.c_str());
    if (!m_map)
    {
        std::cerr << tmx_strerr() << std::endl;
        exit(1);
    }

    world_x = m_map->width;
    world_y = m_map->height;
    tile_w = m_map->tile_width;
    tile_h = m_map->tile_height;

    std::cout << "Creating colliders... " << std::endl;
    tmx_layer* ground(get_layer("ground"));
    if (!ground)
    {
        std::cerr << "Ground layer not found!" << std::endl;
        exit(1);
    }
    for (int i=0; i <(int)world_y; ++i)
    {
        sprites.push_back(std::vector<GameObject*>());
        for (int j = 0; j <(int)world_x; ++j)
        {
            GameObject* sprite(NULL);
            uint32_t gid(ground->content.gids[i*world_x+j]);
            tmx_tile* tile(m_map->tiles[gid]);
            if (tile)
            {
                SDL_Rect r = {
                    (Sint16)(j*tile_w), (Sint16)(i*tile_h),
                    (Uint16)tile_w, (Uint16)tile_h

                };
                sprite = new GameObject(r);
            }
            sprites[i].push_back(sprite);
        }
    }

    std::cout << "Map created!" << std::endl;
}

Map::~Map()
{
    ObjectFactory::destroy();
    Group::clear_buffer();
    tmx_map_free(m_map);
}
void Map::draw_all_layers(tmx_layer* layer, SDL_Surface* screen)
{
    SDL_Rect p;
    while (layer != NULL)
    {
        switch (layer->type)
        {
        case L_GROUP:
            draw_all_layers(layer->content.group_head, screen);
            break;
        case L_LAYER:
            draw_tiles(layer->content.gids, screen);
            break;
        case L_IMAGE:
            p.x = layer->offsetx;
            p.y = layer->offsety;
            SDL_BlitSurface((SDL_Surface*)layer->content.image->resource_image,
                            NULL, screen, &p);
            break;
        default:
            break;
        }
        layer = layer->next;
    }
}

void Map::draw_tiles(uint32_t* gids, SDL_Surface* screen)
{
    int x_min = viewport->y / (int)tile_h,
        x_max = (viewport->y+viewport->h) / (int)tile_h,
        y_min = viewport->x / (int)tile_w,
        y_max = (viewport->x+viewport->w) / (int)tile_w;

    for (int i = x_min; i <= x_max; ++i)
    {
        for (int j = y_min; j <= y_max; ++j)
        {
            if (i<0 or i>=(int)world_y or j<0 or j>=(int)world_x)
                continue;
            unsigned int gid(gids[i*world_x+j] & TMX_FLIP_BITS_REMOVAL);
            tmx_tile* tile(m_map->tiles[gid]);
            if (tile)
            {
                tmx_image* spritesheet(tile->image);
                tmx_tileset* tileset(tile->tileset);

                SDL_Rect subrect = {
                    (Sint16)tile->ul_x, (Sint16)tile->ul_y,
                    (Uint16)tileset->tile_width, (Uint16)tileset->tile_height
                }, dest = { (Sint16)(j*tileset->tile_width), (Sint16)(i*tileset->tile_height) };
                dest.x -= viewport->x;
                dest.y -= viewport->y;

                SDL_Surface* image = (SDL_Surface*)(spritesheet?spritesheet->resource_image:tileset->image->resource_image);
                unsigned int flags(gid & ~TMX_FLIP_BITS_REMOVAL);
                /* TODO : manage flags */
                SDL_BlitSurface(image, &subrect, screen, &dest);
            }
        }
    }
}

void Map::draw(SDL_Surface* screen)
{
    draw_all_layers(m_map->ly_head, screen);
    draw_sprites(screen);
}

void Map::update()
{
    clamp_shift_coords();

    int x_min = viewport->y/(int)tile_h,
        x_max = (viewport->y + viewport->h)/(int)tile_h,
        y_min = viewport->x/(int)tile_w,
        y_max = (viewport->x + viewport->w)/(int)tile_w;

    for (int i=x_min; i<=x_max; ++i)
        for (int j=y_min; j<=y_max ;++j)
        {
            if (!((0<=i and i<(int)world_y) and (0<=j and j<(int)world_x)))
                continue;
            if (sprites[i][j])
                sprites[i][j]->update();
        }
}

void Map::draw_sprites(SDL_Surface* screen)
{
    int x_min = viewport->y/tile_h,
        x_max = (viewport->y + viewport->h)/tile_h,
        y_min = viewport->x/tile_w,
        y_max = (viewport->x + viewport->w)/tile_w;

    for (int i=x_min; i<=x_max; ++i)
        for (int j=y_min; j<=y_max ;++j)
        {
            if (!((0<=i and i<(int)world_y) and (0<=j and j<(int)world_x)))
                continue;
            if (sprites[i][j])
                sprites[i][j]->draw(screen);
        }
}

void Map::move(int x_offset, int y_offset)
{
    viewport->x += (Sint16)x_offset;
    viewport->y += (Sint16)y_offset;
    clamp_shift_coords();
}

bool Map::collision_with(GameObject* sprite)
{
    GameObject* cur_sprite(NULL);

    int x(sprite->get_x()),
        y(sprite->get_y()),
        right(sprite->get_right()),
        bottom(sprite->get_bottom());
    int bx((world_x-1)*tile_w), by((world_y-1)*tile_h);
    if ((bx<=x or x<=0) or (by<=y or y<=0))
        return true;

    int x_min = y/tile_h,
        x_max = bottom/tile_h,
        y_min = x/tile_w,
        y_max = right/tile_w;
    if ((x_min<0 or y_min<0) or (y_max>=(int)world_x or x_max>=(int)world_y))
        return true;

    for (int i=x_min; i<=x_max; ++i)
        for (int j=y_min; j<=y_max ;++j)
        {
            cur_sprite = sprites[i][j];
            if (cur_sprite == sprite or !get_tile_nbr(j, i, false))
                continue;
            if (cur_sprite)
                if (cur_sprite->collide_with(sprite) and
                    sprite->collide_with(cur_sprite))
                    return sprites_collide(sprite, cur_sprite);
        }
    return false;
}

int Map::get_tile_nbr(int x, int y, bool pixel, std::string name)
{
    tmx_layer* layer = get_layer(name);
    if (!layer)
        return 0;

    int i(y), j(x);
    if (pixel)
    {
        i /= tile_h;
        j /= tile_w;
    }
    if (i<0 or j<0 or i>=(int)world_y or j>=(int)world_x)
        return 0;

    return layer->content.gids[i*world_x+j];
}

int Map::get_xshift() { return viewport->x; }
int Map::get_yshift() { return viewport->y; }
int Map::get_tile_w() { return tile_w; }
int Map::get_tile_h() { return tile_h; }
SDL_Rect* Map::get_viewport() { return viewport; }

void Map::clamp_shift_coords()
{
    int bx(world_x*tile_w), by(world_y*tile_h);
    if (viewport->x < 0) viewport->x = 0;
    if (viewport->y < 0) viewport->y = 0;
    if (viewport->x+viewport->w >= bx) viewport->x = bx-viewport->w;
    if (viewport->y+viewport->h >= by) viewport->y = by-viewport->h;
}

void Map::center_on(GameObject* sprite)
{
    if (!sprite)
        return;
    /*
    if (viewport->x + 0.5*viewport->w != sprite->get_centerx())
        viewport->x += sgn(sprite->get_centerx() - viewport->x - 0.5*viewport->w);
    if (viewport->y + 0.5*viewport->h != sprite->get_centery())
        viewport->y += sgn(sprite->get_centery() - viewport->y  - 0.5*viewport->h);
    */
    viewport->x = (Sint16)(sprite->get_centerx() - viewport->w/2.);
    viewport->y = (Sint16)(sprite->get_centery() - viewport->h/2.);

    clamp_shift_coords();
}

void Map::center_on(GameObject* sprite, SDL_Rect limit)
{
    if (!sprite)
        return;
    int center_x = (sprite->get_x()+sprite->get_right())/2,
        center_y = (sprite->get_y()+sprite->get_bottom())/2,
        x_min = viewport->x+limit.x,
        y_min = viewport->y+limit.y;
    int x_max = x_min+limit.w, y_max = y_min+limit.h;
    if (center_x<x_min)
        viewport->x -= (x_min-center_x);
    if (center_y<y_min)
        viewport->y -= (y_min-center_y);
    if (center_x>x_max)
        viewport->x -= (x_max-center_x);
    if (center_y>y_max)
        viewport->y -= (y_max-center_y);
    clamp_shift_coords();
}

tmx_layer* Map::get_layer(const std::string& name)
{
    tmx_layer* ret(m_map->ly_head);
    while (ret)
    {
        if (std::string(ret->name) == name)
            break;
        ret = ret->next;
    }
    return ret;
}

bool Map::sprites_collide(GameObject* sprite, GameObject* cur_sprite)
{
    // override
    return true;
}
