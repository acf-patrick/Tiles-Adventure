#include <sstream>
#include "base/func_tool.h"
#include "base/creator.h"
#include "level.h"
#include "enemies.h"
#include "traps.h"
#include "items.h"

typedef std::map< std::string, std::vector<SDL_Rect> >::iterator tab_iterator;

void Level::load_objects() { __load_objects(m_map->ly_head); }

Level::Level(): Map("maps/runner.tmx")
{
    setup_creators();
    for (int i = 0; i<(int)world_y; ++i)
        for (int j=0; j<(int)world_x; ++j)
        {
            tmx_tile* tile(m_map->tiles[get_tile_nbr(j, i, false)]);
            if (!tile)
                continue;
            if ((17 <= tile->id and tile->id <= 19) or
                (39 <= tile->id and tile->id <= 41) or
                (61 <= tile->id and tile->id <= 63))
                sprites[i][j] = new Poutre(j*tile_w, i*tile_h);
        }
}

Level::~Level()
{
    Basic_fan::destroy_bubbles();
}

void Level::setup_creators()
{
    /* create factory */

    ObjectFactory::registre_creator("AngryPig", new AngryPig::Creator);
    ObjectFactory::registre_creator("Bunny", new Bunny::Creator);
    ObjectFactory::registre_creator("Chicken", new Chicken::Creator);
    ObjectFactory::registre_creator("Chameleon", new Chameleon::Creator);
    ObjectFactory::registre_creator("Duck", new Duck::Creator);
    ObjectFactory::registre_creator("Ghost", new Ghost::Creator);
    ObjectFactory::registre_creator("Mushroom", new Mushroom::Creator);
    ObjectFactory::registre_creator("Plant", new Plant::Creator);
    ObjectFactory::registre_creator("Rino", new Rino::Creator);
    ObjectFactory::registre_creator("Slime", new Slime::Creator);
    ObjectFactory::registre_creator("Skull", new Skull::Creator);
    ObjectFactory::registre_creator("Trunk", new Trunk::Creator);
    ObjectFactory::registre_creator("Turtle", new Turtle::Creator);

    ObjectFactory::registre_creator("End", new End::Creator);
    ObjectFactory::registre_creator("Box", new Box::Creator);
    ObjectFactory::registre_creator("Start", new Start::Creator);
    ObjectFactory::registre_creator("Fruits", new Fruit::Creator);
    ObjectFactory::registre_creator("Checkpoint", new Checkpoint::Creator);

    ObjectFactory::registre_creator("Arrow", new Arrow::Creator);
    ObjectFactory::registre_creator("Falling platform", new Falling_platform::Creator);
    ObjectFactory::registre_creator("Fan", new Fan::Creator);

    /* set up parameters */
    ObjectCreator::addToParameters(this, "game map");
    ObjectCreator::addToParameters(viewport, "game viewport");
    ObjectCreator::addToParameters(&bullets, "bullets group");
}

void Level::add_enemies(GameObject* enemy)
{
    enemies.add(enemy);
}
void Level::add_item(GameObject* item)
{
/*
    if (item->is("Checkpoints"))
    {
        item->set_x(((item->get_x())/tile_w)*tile_w);
        item->set_y(((item->get_y())/tile_w)*tile_w);
    }
*/
    items[item->get_ancestor()].add(item);
}
void Level::add_traps(GameObject* trap)
{
    traps.add(trap);
}

void Level::delete_sprite_at(int x, int y)
{
    x += viewport->x;
    y += viewport->y;
    GameObject* sprite = enemies.first_sprite_colliding_with(x, y);
    if (sprite)
        enemies.remove(sprite);
    else
    {
        sprite = traps.first_sprite_colliding_with(x, y);
        if (sprite)
            traps.remove(sprite);
        else
            for (std::map<std::string, Group>::iterator it=items.begin();
                it!=items.end(); ++it)
            {
                sprite = it->second.first_sprite_colliding_with(x, y);
                if (sprite)
                    it->second.remove(sprite);
            }
    }
}

void Level::update()
{
    clamp_shift_coords();
    enemies.update();
    traps.update();
    Basic_fan::update_bubbles(this);
    dying.update();
    bullets.update();
    for (std::map<std::string ,Group>::iterator it=items.begin();
        it != items.end(); ++it)
        it->second.update();
}

void Level::draw(SDL_Surface* screen)
{
    for (std::map<std::string ,Group>::iterator it=items.begin();
        it != items.end(); ++it)
        it->second.draw(screen);
    traps.draw(screen);
    Basic_fan::draw_bubbles(screen);
    Map::draw(screen);
    enemies.draw(screen);
    dying.draw(screen);
    bullets.draw(screen);
}

bool Level::collision_with(GameObject* sprite)
{
    if (sprite->is("Player"))
    {
        GameObject* trap = traps.first_sprite_colliding_with(sprite);
        GameObject* enemy = enemies.first_sprite_colliding_with(sprite);
        GameObject* bullet = bullets.first_sprite_colliding_with(sprite);
        GameObject* box = items["Boxes"].first_sprite_colliding_with(sprite);
        GameObject* fruit = items["Fruits"].first_sprite_colliding_with(sprite);
        GameObject* checkpoint = items["Checkpoints"].first_sprite_colliding_with(sprite);
        if (enemy)
        {
            if (enemy->is("Turtle") and enemy->check_up /* i.e Spikes out */)
                sprite->bump("die");
            else
            {
                // le personnage est entrain de tomber
                if (sprite->check_down
                    // le personnage est au-dessus de l'enemie ... primordiale!!!
                )//    and abs(sprite->get_bottom() - enemy->get_y()) <= 10)
                {
                    sprite->bump();
                    remove_enemy(enemy);
                }
                else
                    sprite->bump("die");
            }
        }
        if (fruit)
            fruit->bump();
        if (checkpoint)
            checkpoint->bump();
        if (box)
            if (box->is("Boxes") and sprite->check_down)
            {
                std::stringstream oss;
                oss << sprite->get_impulse()[1];
                box->bump(oss.str());
                if (!items["Boxes"].has(box))
                {
                    std::string name[] = { "Apple", "Bananas", "Cherries", "Kiwi",
                                        "Melon", "Orange", "Pineapple", "Strawberry" };
                    int n(randint(1, 5));
                    for (int i=0; i<n; ++i)
                        items["Fruits"].add(new Fruit(box->get_x(), box->get_y()-7, this, name[rand()%8], true));
                }
                sprite->bump("box repulsion");
            }
        if (bullet)
            if (bullet->is("Bullet"))
            {
                if (sprite->check_down)
                    bullet->bump("break");
                else
                {
                    bullet->bump();
                    sprite->bump("die");
                }
            }
        if (trap)
        {
            if (sprite->check_down)
            {
                if (trap->is("Arrow"))
                    sprite->bump("arrow repulsion");
                else  if (trap->is("Falling Platform"))
                    sprite->bump("platform repulsion");
                trap->bump();
            }
        }
    }
    else if (!sprite->is("Plant") and !sprite->is("Trunk"))
        if (enemies.has(sprite))
        {
            GameObject* bullet = bullets.first_sprite_colliding_with(sprite);
            if (bullet)
            {
                bullet->bump();
                if (sprite->is("AngryPig"))
                    sprite->bump("shotted");
                else
                    remove_enemy(sprite);
            }
        }
    return Map::collision_with(sprite);
}

void Level::remove_enemy(GameObject* enemy)
{
    enemy->bump();
    enemies.remove(enemy);
    dying.add(enemy);
}

void Level::__load_objects(tmx_layer* layer)
{
    while (layer)
    {
        ObjectCreator::addToParameters(layer, "current layer");
        if (layer->type == L_GROUP)
            __load_objects(layer->content.group_head);
        else if (layer->type == L_OBJGR)
        {
            for (tmx_object* object = layer->content.objgr->head;
                 object; object = object->next)
            {
                ObjectCreator::addToParameters(object, "current object");
                int x(object->x), y(object->y - object->height);
                GameObject* obj(ObjectFactory::create(tmx_get_property(layer->properties, "fruit")?"Fruits":std::string(layer->name), x, y));
                std::string type(tmx_get_property(layer->properties, "type")->value.string);
                if (type == "Enemies")
                    add_enemies(obj);
                else if (type == "Items")
                    add_item(obj);
                else if (type == "Traps")
                    add_traps(obj);
            }
        }
        layer = layer->next;
    }
}
