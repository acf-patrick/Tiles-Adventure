#include <SDL_image.h>
#include <fstream>
#include <sstream>
#include <utility>
#include <algorithm>
#include <SDL_rotozoom.h>
#include "map.h"
#include "app.h"
#include "func_tool.h"

SDL_Rect Map::camera = {0, 0, 0, 0};

Map::Map(const std::string& map_file_name, SDL_Rect* rect):
    Loader(map_file_name)
{
    sprites.resize(m_map.size());
    for (int i=0; i<(int)m_map.size(); ++i)
        sprites[i].resize(m_map[i].size(), NULL);

    IMG_Init(IMG_INIT_PNG);

    if (rect)
        viewport = rect;

    else
    {
        viewport = new SDL_Rect;
        viewport->x = 0;
        viewport->y = 0;
        viewport->w = App::width;
        viewport->h = App::height;
    }
    init_tilesets();
    for (int i=0; i<(int)m_tileset.size(); ++i)
        for (int j=0; j<(int)tileset[i]->m_names.size(); ++j)
            names.push_back(tileset[i]->m_names[j]);

    std::cout << "map created successfully\n";
}

Map::~Map() { Group::clear_buffer(); }

void Map::init_tilesets()
{
    for (int i=0; i<(int)m_tileset.size(); ++i)
    {
        std::cout << m_tileset[i] << std::endl;
        Tileset* t = new Tileset(m_tileset[i].substr(0, m_tileset[i].size()-4));
        original_spritesheet.push_back(IMG_Load(m_tileset[i].c_str()));
        t->tile_x = t->surface->w/tile_w;
        t->tile_y = t->surface->h/tile_h;
        tileset.push_back(t);
        if (i == 0)
            intervals.push_back(std::pair<int, int>(1, t->tile_x*t->tile_y));
        else
            intervals.push_back(std::pair<int, int>(intervals[i-1].second+1, intervals[i-1].second+t->tile_x*t->tile_y));
    }
}

void Map::draw(SDL_Surface* screen, bool layer_by_layer)
{
    if (layer_by_layer)
        for (int i=0; i<(int)layers.size(); ++i)
            draw_layer(screen, layers[i]);
    else
        draw_layer(screen, m_map);
    draw_sprites(screen);
}

void Map::update()
{
    clamp_shift_coords();

    int x_min = viewport->y/tile_h,
        x_max = (viewport->y + viewport->h)/tile_h,
        y_min = viewport->x/tile_w,
        y_max = (viewport->x + viewport->w)/tile_w;

    for (int i=x_min; i<=x_max; ++i)
        for (int j=y_min; j<=y_max ;++j)
        {
            if (!((0<=i and i<world_y) and (0<=j and j<world_x)))
                continue;
            if (sprites[i][j])
                sprites[i][j]->update();
        }
}

void Map::scale(float zoom)
{
    if (zoom <= 0)
        return;

    for (int i=0; i<(int)tileset.size(); ++i)
        tileset[i]->surface = rotozoomSurface(original_spritesheet[i], 0, zoom, true);
    tile_w *= zoom;
    tile_h *= zoom;
    std::cout << "tile size : " << tile_w << ", " << tile_h << std::endl;
    std::cout << "Spritesheets size : \n";
    for (int i=0; i<(int)tileset.size(); ++i)
        std::cout << tileset[i]->surface->w << ", " << tileset[i]->surface->h << std::endl;
}

void Map::move(int x_offset, int y_offset)
{
    viewport->x += (Sint16)x_offset;
    viewport->y += (Sint16)y_offset;
    clamp_shift_coords();
}

int Map::get_xshift() { return viewport->x; }
int Map::get_yshift() { return viewport->y; }
int Map::get_tile_w() { return tile_w; }
int Map::get_tile_h() { return tile_h; }
SDL_Rect* Map::get_viewport() { return viewport; }
SDL_Surface* Map::get_spritesheet(const std::string& name)
{
    int index;
    for (index = 0; index < (int)m_tileset.size(); ++index)
        if (m_tileset[index].find(name) != name.npos)
            break;
    if (index == (int)m_tileset.size())
        return NULL;
    return original_spritesheet[index];
}
std::string Map::get_tile(int x, int y, int layer_index)
{
    std::vector< std::vector<int> > layer((layer_index<0 or layer_index>=(int)layers.size())?m_map:layers[layer_index]);
    int i(y/tile_h), j(x/tile_w);
    if (i<0 or j<0 or i>=world_y or j>=world_x)
        return "blank";

    int nbr(layer[i][j]);
    if (!nbr)
        return "blank";
    return names[nbr-1];
}
int Map::get_tile_nbr(int x, int y, int layer_index)
{
    std::vector< std::vector<int> > layer((layer_index<0 or layer_index>=(int)layers.size())?m_map:layers[layer_index]);
    int i(y/tile_h), j(x/tile_w);
    if (i<0 or j<0 or i>=world_y or j>=world_x)
        return 0;
    return layer[i][j];
}
std::pair<int, int> Map::get_tile_coordinate(const std::string& tile_name)
{
    std::pair<int, int> ret;
    ret.first  = -1;
    ret.second = -1;
    int index_in_names(0);
    while (names[index_in_names] != tile_name and index_in_names < (int)names.size())
        index_in_names++;
    if (index_in_names == (int)names.size())
        return ret;

    int index_in_intervals(0);
    while (intervals[index_in_intervals].first>index_in_names or index_in_names>intervals[index_in_intervals].second)
        index_in_intervals++;

    int index_in_tileset(index_in_names+1 - intervals[index_in_intervals].first);
    ret.first = index_in_tileset%tileset[index_in_intervals]->tile_x;
    ret.second = index_in_tileset/tileset[index_in_intervals]->tile_x;
    return ret;
}
int Map::get_tile_index(const std::string& tile_name)
{
    int index_in_names(0);
    while (names[index_in_names] != tile_name and index_in_names < (int)names.size())
        index_in_names++;
    if (index_in_names == (int)names.size())
        return 0;
    return index_in_names;
}

void Map::draw_layer(SDL_Surface* screen, std::vector< std::vector<int> > layer, bool drawing_decor)
{
    int i, j;
    int x_min = viewport->y / tile_h,
        x_max = (viewport->y+viewport->h) / tile_h,
        y_min = viewport->x / tile_w,
        y_max = (viewport->x+viewport->w) / tile_w;

    SDL_Rect dest, pos;
    dest.w = tile_w;
    dest.h = tile_h;

    for (i=x_min; i<=x_max; ++i)
        for (j=y_min; j<=y_max; ++j)
        {
            if (!((0<=i and i<world_y) and (0<=j and j<world_x)))
                continue;
            pos.x = j*tile_w - viewport->x;
            pos.y = i*tile_h - viewport->y;
            int nbr(layer[i][j]);
            if (!nbr) continue;

            if (drawing_decor and !is_decor(names[layer[i][j]-1]))
                    continue;

            int k(0);
            while (intervals[k].first > nbr or nbr > intervals[k].second)
                k++;
            nbr -= intervals[k].first;

            dest.x = (Sint16)(nbr%tileset[k]->tile_x)*tile_w;
            dest.y = (Sint16)(nbr/tileset[k]->tile_x)*tile_h;

            SDL_BlitSurface(tileset[k]->surface, &dest, screen, &pos);
        }
}


void Map::draw_decor(SDL_Surface* screen)
{
    // override
}

void Map::draw_decor(SDL_Surface* screen, const std::vector< std::vector<int> >& layer)
{
    draw_layer(screen, layer, true);
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
            if (!((0<=i and i<world_y) and (0<=j and j<world_x)))
                continue;
            if (sprites[i][j])
                sprites[i][j]->draw(screen);
        }
}

bool Map::collision_with(Sprite* sprite)
{
    Sprite* cur_sprite(NULL);

    int x(sprite->get_x()),
        y(sprite->get_y()),
        right(sprite->get_right()),
        bottom(sprite->get_bottom());
    if (((world_x-1)*tile_w<=x or x<=0) or ((world_y-1)*tile_h<=y or y<=0))
        return true;

    int x_min = y/tile_h,
        x_max = bottom/tile_h,
        y_min = x/tile_w,
        y_max = right/tile_w;
    if ((x_min<0 or y_min<0) or (y_max>=world_x or x_max>=world_y))
        return true;

    for (int i=x_min; i<=x_max; ++i)
        for (int j=y_min; j<=y_max ;++j)
        {
            cur_sprite = sprites[i][j];
            if (cur_sprite == sprite or !m_map[i][j])
                continue;
            if (cur_sprite)
                if (cur_sprite->collide_with(sprite) and
                    sprite->collide_with(cur_sprite))
                    return sprites_collide(sprite, cur_sprite);
        }
    return false;
}

bool Map::groups_collide_with(Sprite* sprite)
{
    // override
    return false;
}

bool Map::sprites_collide(Sprite* sprite, Sprite* cur_sprite)
{
    return true;
}

void Map::center_on(Sprite* sprite)
{
    if (!sprite)
        return;
    viewport->x = (Sint16)(sprite->get_centerx() - viewport->w/2.);
    viewport->y = (Sint16)(sprite->get_centery() - viewport->h/2.);
    clamp_shift_coords();
}

void Map::clamp_shift_coords()
{
    if (viewport->x < 0) viewport->x = 0;
    if (viewport->y < 0) viewport->y = 0;
    if (viewport->x+viewport->w >= world_x*tile_w) viewport->x = world_x*tile_w-viewport->w;
    if (viewport->y+viewport->h >= world_y*tile_h) viewport->y = world_y*tile_h-viewport->h;
}

void Map::center_on(Sprite* sprite, SDL_Rect limit)
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

void Map::set(int x, int y, int value)
{
    if (x>=world_y or y>=world_x)
        return;
    m_map[x][y] = value;
}

void Map::clear(int x, int y)
{
    delete sprites[x][y];
    sprites[x][y] = NULL;
    for (int i=0; i<(int)layers.size(); ++i)
        layers[i][x][y] = 0;
    m_map[x][y] = 0;
}

void Map::reset_spritesheet(int index)
{
    if (index >= 0)
    {
        tileset[index]->surface = original_spritesheet[index];
        return;
    }
    for (int i=0; i<(int)tileset.size(); ++i)
        tileset[i]->surface = original_spritesheet[i];
}

bool Map::is_decor(const std::string& name)
{
    // override
    return false;
}
