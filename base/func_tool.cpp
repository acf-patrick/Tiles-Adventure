#include <SDL_image.h>
#include <fstream>
#include <sstream>
#include <numeric>
#include <cmath>
#include <ctime>
#include "func_tool.h"
#include "timer.h"

#ifndef M_PI
#define M_PI	3.1415926535897932384626433832795
#endif

std::string replace(const std::string& chaine, const std::string& substring, const std::string& new_substring)
{
    int pos(chaine.find(substring));
    if (pos < 0)
        return chaine;
    std::string part_1(chaine.substr(0, pos)),
                part_2(chaine.substr(pos+substring.size()));
    return replace(std::string(part_1+new_substring+part_2), substring, new_substring);
}

std::string strip(const std::string& _chaine)
{
    std::string chaine(_chaine);
    int pos[2] = {0, (int)_chaine.size()-1};
    for (int i=0; i<2; ++i)
        while (true)
        {
            if (chaine[pos[i]] == ' ' or chaine[pos[i]] == '\t')
            {
                chaine.erase(pos[i], 1);
                if (i)
                    pos[i]--;
            }
            else if (chaine[pos[i]] == '\0')
                pos[i] = chaine.size()-1;
            else
                break;
        }
    return chaine;
}

std::vector<std::string> split(const std::string& chaine, const std::string& sep)
{
    std::vector<std::string> ret;
    int start_pos(0), pos;
    while((pos = chaine.find(sep, start_pos)) >= 0)
    {
        if (pos != start_pos)
            ret.push_back(chaine.substr(start_pos, pos-start_pos));
        start_pos = pos+1;
    }
    ret.push_back(chaine.substr(chaine.find_last_not_of(sep, start_pos)));
    return ret;
}

int sgn(double nbr) { return (nbr == 0)?0:(nbr>0)?1:-1; }

bool rects_collide(const SDL_Rect& rect1, const SDL_Rect& rect2)
{
    return (
            rect1.y+rect1.h > rect2.y and
            rect1.y < rect2.y+rect2.h and
            rect1.x+rect1.w > rect2.x and
            rect1.x < rect2.x+rect2.w
            );
}
bool rect_n_point_collide(const SDL_Rect& rect, int x, int y)
{
    SDL_Rect point_rect = {Sint16(x), Sint16(y), 0};
    return rects_collide(rect, point_rect);
}
bool circle_n_rect_collide(const Circle& circle, const SDL_Rect& rect)
{
    SDL_Rect circle_rect = {Sint16(circle.center_x-circle.radius), Sint16(circle.center_y-circle.radius), Uint16(circle.radius*2), Uint16(circle.radius*2)};
    if (!rects_collide(circle_rect, rect)) return false;
    if (
        circle_n_point_collide(circle, rect.x, rect.y) or
        circle_n_point_collide(circle, rect.x, rect.y+rect.h) or
        circle_n_point_collide(circle, rect.x+rect.w, rect.y) or
        circle_n_point_collide(circle, rect.x+rect.w, rect.y+rect.h)
        ) return true;
    if (rect_n_point_collide(rect, circle.center_x, circle.center_y)) return true;
    if (
        project_on_seg(circle.center_x, circle.center_y, rect.x, rect.y, rect.x, rect.y+rect.h) or
        project_on_seg(circle.center_x, circle.center_y, rect.x, rect.y, rect.x+rect.w, rect.y)
        ) return true;
    return false;
}
bool circle_n_point_collide(const Circle& circle, int x, int y)
{
    SDL_Rect point = {Sint16(x), Sint16(y)};
    return circle_n_point_collide(circle, point);
}
bool circle_n_point_collide(const Circle& circle, const SDL_Rect& point)
{
    return (std::sqrt((circle.center_x-point.x)*(circle.center_x-point.x)+(circle.center_y-point.y)*(circle.center_y-point.y)) <= circle.radius);
}

bool project_on_seg(double c_x, double c_y, int a_x, int a_y, int b_x, int b_y)
{
    double ac_x(c_x-a_x), ac_y(c_y-a_y), ab_x(b_x-a_x), ab_y(b_y-a_y), bc_x(c_x-b_x), bc_y(c_y-b_y);
    //dot product
    double s1(ac_x*ab_x+ac_y*ab_y), s2(bc_x*ab_x+bc_y*ab_y);
    return (s1*s2 <= 0);
}

bool seg_n_seg_collide(int a1_x, int a1_y, int b1_x, int b1_y, int a2_x, int a2_y, int b2_x, int b2_y, double *inter_x, double *inter_y)
{
    float a1, b1, a2, b2;

    a1 = a1_x - b1_x;
    if (a1)
        a1 = (a1_y-b1_y)/a1;
    else
        a1 = 0;
    b1 = a1_y - a1*a1_x;

    a2 = a2_x - b2_x;
    if (a2)
        a2 = (a2_y-b2_y)/a2;
    else
        a2 = 0;
    b2 = a2_y - a2*a2_x;

    // les deux ségments sont parallèles
    if (a1 == a2)
        return false;

    double p_x = (b1-b2)/(a2-a1);
    double p_y = a1*p_x + b1;

    if (inter_x and inter_y)
    {
        *inter_x = p_x;
        *inter_y = p_y;
    }

    return project_on_seg(p_x, p_y, a1_x, a1_y, b1_x, b2_y);
}

Uint32 getPixel(SDL_Surface* surface, int x, int y)
{
    if (!surface)
        return 0;
    if (x<0 or y<0 or x>=surface->w or y>=surface->h)
        return 0;
    int nbOctetsParPixel = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8*)surface->pixels + y*surface->pitch + x*nbOctetsParPixel;
    switch (nbOctetsParPixel)
    {
    case 1:
        return *p;
    case 2:
        return *(Uint16*)p;
    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return  p[0] << 16|p[1] << 8|p[2];
        else
            return p[0]|p[1] << 8|p[2] << 16;
    case 4:
        return *(Uint32*)p;
    default:
        return 0;
    }
}
void setPixel(SDL_Surface* surface, int x, int y, Uint32 pixel)
{
    if (!surface)
        return;
    int nbOctetsParPixel = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8*)surface->pixels + y*surface->pitch + x*nbOctetsParPixel;
    switch (nbOctetsParPixel)
    {
    case 1:
        *p = pixel;
        break;
    case 2:
        *(Uint16*)p = pixel;
        break;
    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
        {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8)  & 0xff;
            p[2] = pixel & 0xff;
        }
        else
        {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8)  & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;
    case 4:
        *(Uint32*)p = pixel;
        break;
    }
}
SDL_Surface* flip(SDL_Surface* source, bool x_flip, bool y_flip)
{
    if (!source)
        return NULL;
    SDL_Surface* ret = SDL_CreateRGBSurface(SDL_HWSURFACE, source->w, source->h, 32,
                                            source->format->Rmask, source->format->Gmask, source->format->Bmask, source->format->Amask);
    if (!ret)
        return NULL;

    for (int i = 0; i<source->h; ++i)
        for (int j=0; j<source->w; ++j)
            setPixel(ret, x_flip?(source->w - (j+1)):j, y_flip?(source->h - (i+1)):i, getPixel(source, j, i));

    return ret;
}

SDL_Surface* black_n_white(SDL_Surface* source)
{
    if (!source)
        return NULL;
    SDL_Surface* ret = SDL_CreateRGBSurface(SDL_HWSURFACE, source->w, source->h, 32,
                                            source->format->Rmask, source->format->Gmask, source->format->Bmask, source->format->Amask);
    if (!ret)
        return NULL;

    for (int i=0; i<source->h; ++i)
        for (int j=0; j<source->w; ++j)
        {
            Uint8 r, g, b, a;
            SDL_GetRGBA(getPixel(source, j, i), source->format, &r, &g, &b, &a);
            Uint32 p; // = SDL_MapRGBA(source->format, 0, 0, 0, a?255:0);
            r = (r+g+b)/3;
            p = SDL_MapRGBA(source->format, r, r, r, a);
            setPixel(ret, j, i, p);
        }
    return ret;
}

void intervals_intersection(int a0, int b0, int a1, int b1, int *a, int *b)
{
    if (b0<a0) return intervals_intersection(b0, a0, a1, b1, a, b);
    if (b1<a1) return intervals_intersection(a0, b0, b1, a1, a, b);
    if (a1<=a0 and a0<=b0 and b0<=b1) { *a = a0; *b = b0; }
    else if (a0<=a1 and a1<=b1 and b1<=b0) { *a = a1; *b = b1; }
    else if (a0<=a1) { *a = a1; *b = b0; }
    else if (a1<=a0) { *a = a0; *b = b1; }
}

SDL_Rect* clip_rects(const SDL_Rect& rect1, const SDL_Rect& rect2)
{
    if (!rects_collide(rect1, rect2))
        return NULL;

    int x0, y0, x1, y1;
    intervals_intersection(rect1.x, rect1.x+rect1.w, rect2.x, rect2.x+rect2.w, &x0, &x1);
    intervals_intersection(rect1.y, rect1.y+rect1.h, rect2.y, rect2.y+rect2.h, &y0, &y1);
    SDL_Rect *ret = new SDL_Rect;
    ret->x = Sint16(x0);
    ret->y = Sint16(y0);
    ret->w = Uint16(x1-x0);
    ret->h = Uint16(y1-y0);

    return (!ret->w or !ret->h)?NULL:ret;
}

bool isGrey(Uint8 color[3])
{
    return (color[0] == color[1] and color[1] == color[2]);
}

SDL_Surface* get_image(SDL_Surface* spritesheet, Sint16 x, Sint16 y, Uint16 w, Uint16 h)
{
    if (!spritesheet)
        return NULL;
    if (x+w > spritesheet->w)
        w = spritesheet->w - x;
    if (y+h > spritesheet->h)
        h = spritesheet->h - y;
    SDL_Surface* ret;
    SDL_LockSurface(spritesheet);
    ret = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, 32,
                             spritesheet->format->Rmask, spritesheet->format->Gmask, spritesheet->format->Bmask, spritesheet->format->Amask);
    if (!ret)
    {
        SDL_UnlockSurface(spritesheet);
        return NULL;
    }
    for (int i=0; i<w; ++i)
        for (int j=0; j<h; ++j)
            setPixel(ret, i+x, j+y, getPixel(spritesheet, i+x, j+y));
    SDL_UnlockSurface(spritesheet);
    return ret;
}

SDL_Surface* copy_surface(SDL_Surface* original)
{
    if (!original)
        return NULL;
    return get_image(original, 0, 0, original->w, original->h);
}

int randint(int a, int b)
{
    if (a>b) return randint(b, a);
    return ((rand()/(float)RAND_MAX)*(b-a)+a);
}

bool images_collide(int x1, int y1, SDL_Surface* s1, int x2, int y2, SDL_Surface* s2)
{
    if (s1 and s2)
    {
        SDL_Rect r1 = {Sint16(x1), Sint16(y1), Uint16(s1->w), Uint16(s1->h)},
                 r2 = {Sint16(x2), Sint16(y2), Uint16(s2->w), Uint16(s2->h)};
        SDL_Rect *sub(clip_rects(r1, r2));

        if (sub)
        {
            for (int x=0; x<sub->w; ++x)
                for (int y=0; y<sub->h; ++y)
                {
                    Uint32 px1 = getPixel(s1, x+sub->x-x1, y+sub->y-y1),
                           px2 = getPixel(s2, x+sub->x-x2, y+sub->y-y2);
                    Uint8 color[2][3];
                    SDL_GetRGB(px1, s1->format, &color[0][0], &color[0][1], &color[0][2]);
                    SDL_GetRGB(px2, s2->format, &color[1][0], &color[1][1], &color[1][2]);
                    if (!(isGrey(color[0]) or isGrey(color[1])))
                        return true;
                }
        }
    }
    return false;
}

void apply_rotation(int cx, int cy, int angle, int* x, int *y)
{
    if (!x or !y)
        return;
    float alpha = M_PI/180 * angle;
    int _x(*x), _y(*y);
    *x = cx + (_x-cx)*std::cos(alpha) - (_y-cy)*std::sin(alpha);
    *y = cy + (_x-cx)*std::sin(alpha) + (_y-cy)*std::cos(alpha);
}
