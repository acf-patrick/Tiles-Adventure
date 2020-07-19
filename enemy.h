#ifndef ENEMY_H
#define ENEMY_H

#include "base/with_mass.h"
#include "base/object.h"
#include "base/timer.h"
#include "base/group.h"
#include "base/map.h"
#include <vector>
#include <SDL.h>
#include <map>

/** Classe de base pour tous les enemmies dans dans le jeu */
class Enemy : public With_mass
{
public:
    /**
    * @param Les trois premiers parametres sont les parametres de base de la classe With_mass.
    * @param name : nom du personnage
    * @param les deux derniers sont la taille du personnage
    */
    Enemy(Map* ,int ,int ,const std::string& ,int ,int);
    virtual ~Enemy();
    virtual void bump(const std::string& flag = "");
    virtual void draw(SDL_Surface*);
    virtual SDL_Surface* get_surface();

protected:
    // left and right
    std::map<std::string, SDL_Surface*> images[2];
    std::string state;
    int cur_image;
    int delay;
    Timer timer;
    Timer state_timer;

    virtual void update_image();
    void load_images(const std::vector<std::string>&);
    void stop_falling();
    void wait();
    bool erase();
};

/** Classe de base pour les enemies utilisant quelconque projectile dans le jeu*/
class Shot: public Enemy
{
public:
    Shot(const std::string&, Map*, int, int, int, int, GameObject*, Group*);
    void draw(SDL_Surface*);

protected:
    GameObject* target;
    void fire();

private:
    Group* bullets;
};

#endif
