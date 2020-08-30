#ifndef MAP_H
#define MAP_H

#include <vector>
#include <SDL.h>
#include <tmx.h>
#include "creator.h"
#include "object.h"

/// \brief This class manage many of a game map functionalities,
/// such as rendering and collision handling. Also creates automatically
/// ground tiles sprites which some game objects will collide with.
///
/// \todo SET THE 'name' ATTRIBUT OF THE GROUND LAYER TO "ground" (match case)
class Map
{
public:

    /// \param map_file_name : the tmx file to load
    /// \param viewport_rect : a pointer to SDL_Rect. Set to NULL by default, the object will use its own viewport
    Map(const std::string&, SDL_Rect* viewport_rect = NULL);
    virtual ~Map();

    /// \brief render the map
    /// \param screen : a pointer to SDL_Surface. The map will be shown onto this surface.
    virtual void draw(SDL_Surface*);

    /// \brief Update the map object.
    virtual void update();

    /// \param (x_offset, y_offset) : move the object by x_offset and y_offset
    void move(int, int);

    /// \brief Check if there are collision between the map object and Object given as parameter
    virtual bool collision_with(GameObject*);
/*
    /// \brief use to check if there are collision between the Object given as parameter and the map's Group objects.
    /// \todo need to be overriden
    virtual bool groups_collide_with(GameObject*);
*/
    /// \brief Focus on a game object
    virtual void center_on(GameObject*);

    /// \brief Focus on a game object. Maintain the object inside a restricted rectangle.
    virtual void center_on(GameObject*, SDL_Rect);

    /* getters */
    int get_xshift();
    int get_yshift();
    int get_tile_w();
    int get_tile_h();
    SDL_Rect* get_viewport();

    /// \brief Get tile's GID at the given position
    /// \param x, y : position of the cell in global coordinates.
    /// \param pixel : this flag tells if the (x, y) position is in pixels or in cells. pixel is set by default to true.
    /// \param name : name of the layer of the cell.
    /// \return GID of the tile. 0 if the cell is empty (cf : libtmx doc)
    int get_tile_nbr(int, int, bool pixel = true, std::string name = "ground");

    static SDL_Rect camera;

protected:
    SDL_Rect *viewport;
    unsigned int world_x, world_y;
    unsigned int tile_w, tile_h;
    std::vector< std::vector<GameObject*> > sprites;

    /// \brief rectify viewport
    void clamp_shift_coords();

    /// \brief get a layer in the map by its name
    tmx_layer* get_layer(const std::string&);

    /// \brief define here what to do with colliding objects found by the collision_with method.
    virtual bool sprites_collide(GameObject*, GameObject*);
/*
    /// \brief Inside this method, initialize parameters of game object creators
    virtual void setup_creators() = 0;
*/
    tmx_map *m_map;

private:

    void draw_sprites(SDL_Surface*);
    void draw_all_layers(tmx_layer*, SDL_Surface*);
    void draw_tiles(uint32_t*, SDL_Surface*);

friend class ObjectCreator;
};


#endif // MAP_H
