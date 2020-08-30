#ifndef FUNC_TOOL
#define FUNC_TOOL

#include <iostream>
#include <SDL.h>
#include <string>
#include <vector>

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    #define RMASK 0xff000000
    #define GMASK 0x00ff0000
    #define BMASK 0x0000ff00
    #define AMASK 0x000000ff
#else
    #define AMASK 0xff000000
    #define BMASK 0x00ff0000
    #define GMASK 0x0000ff00
    #define RMASK 0x000000ff
#endif

#define createSurface(w, h) SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, 32, RMASK, GMASK, BMASK, AMASK)

int sgn(double);

typedef struct
{
    int center_x, center_y;
    int radius;
}Circle;

SDL_Surface* black_n_white(SDL_Surface*);
SDL_Surface* get_image(SDL_Surface*, Sint16 , Sint16, Uint16, Uint16);
SDL_Surface* copy_surface(SDL_Surface*);
SDL_Surface* flip(SDL_Surface* , bool, bool);

bool project_on_seg(double, double, int, int, int, int);

/* functions for collision detection */
bool rects_collide(const SDL_Rect&, const SDL_Rect&);
bool rect_n_point_collide(const SDL_Rect&, int, int);
bool circle_n_rect_collide(const Circle&, const SDL_Rect&);
bool circle_n_point_collide(const Circle&, const SDL_Rect&);
bool circle_n_point_collide(const Circle&, int, int);
bool seg_n_seg_collide(int a1_x, int a1_y, int b1_x, int b1_y, int a2_x, int a2_y, int b2_x, int b2_y, double *inter_x=NULL, double *inter_y=NULL);

/// \brief Replace all "substring" occurences in given string "chaine" by "new_substring"
std::string replace(const std::string&, const std::string&, const std::string&);

/// \brief remove whitespace head or tail of the string
std::string strip(const std::string&);

/// \brief Return a list of the words in the string "chaine", using "sep" as the delimiter string.
std::vector<std::string> split(const std::string&, const std::string&);

SDL_Rect* clip_rects(const SDL_Rect&, const SDL_Rect&);

/// \brief Returns a random number in [a, b[
int randint(int a, int b);

/* Using grey mask */
bool images_collide(int, int, SDL_Surface*, int, int, SDL_Surface*);

/// \brief apply a rotation to a point
/// \param (cx, cy) : coordinates of the center point
/// \param angle : angle in degrees
/// \param (x, y) : coordinates of the point to apply rotation
void apply_rotation(int, int, int, int*, int*);
#endif
