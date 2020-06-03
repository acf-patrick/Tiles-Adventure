#include <fstream>
#include <sstream>
#include "fruits.h"
#include "level.h"

typedef std::map< std::string, std::vector<SDL_Rect> >::iterator tab_iterator;

Level::Level(): Map("map.txt")
{
    for (int i = 0; i<world_y; ++i)
        for (int j=0; j<world_x; ++j)
            if (m_map[i][j])
            {
                SDL_Rect r = {Sint16(j*tile_w), Sint16(i*tile_h), tile_w, tile_h};
                sprites[i][j] = new Sprite(r);
            }
}

Level::~Level()
{
    std::ofstream file("level.acf");

    file << "Enemies\n{\n";
    for (tab_iterator it=enemies_pos.begin();
        it != enemies_pos.end(); ++it)
    {
        file << '\t' << it->first << "\n\t{\n";
        for (int i=0; i<(int)it->second.size(); ++i)
        {
            file << "\t\t" << it->second[i].x << ", " << it->second[i].y << std::endl;
        }
        file << "\t}\n";
    }
    file << "}\n";

    file << "Items\n{\n";
    for (std::map< std::string, std::map< std::string, std::vector<SDL_Rect> > >::iterator i=items_pos.begin();
        i != items_pos.end(); ++i)
    {
        file << '\t' << i->first << "\n\t{\n";
        for (tab_iterator it=i->second.begin(); it!=i->second.end(); ++it)
        {
            file << "\t\t" << it->first << "\n\t\t{\n";
            for (int k=0; k<(int)it->second.size(); ++k)
                file << "\t\t\t" << it->second[k].x << ", " << it->second[k].y << '\n';
            file << "\t\t}\n";
        }
        file << "\t}\n";
    }
    file << "}\n";
}

void Level::add_enemies(Sprite* enemy)
{
    enemies.add(enemy);
    enemies_pos[enemy->get_type()].push_back(enemy->get_rect());
}
void Level::add_item(Sprite* item)
{
    if (item->is("Checkpoints"))
    {
        item->set_x(((item->get_x())/tile_w)*tile_w);
        item->set_y(((item->get_y())/tile_w)*tile_w);
    }
    items[item->get_ancestor()].add(item);
    items_pos[item->get_ancestor()][item->get_type()].push_back(item->get_rect());
}
void Level::delete_sprite_at(int x, int y)
{
    x += viewport->x;
    y += viewport->y;
    Sprite* sprite = enemies.first_sprite_colliding_with(x, y);
    if (sprite)
        enemies.remove(sprite);
    else
        for (std::map<std::string, Group>::iterator it=items.begin();
            it!=items.end(); ++it)
        {
            sprite = it->second.first_sprite_colliding_with(x, y);
            if (sprite)
                it->second.remove(sprite);
        }
}

void Level::update()
{
    enemies.update();
    dying.update();
    bullets.update();
    for (std::map<std::string ,Group>::iterator it=items.begin();
        it != items.end(); ++it)
        it->second.update();
}

void Level::draw(SDL_Surface* screen)
{
    Map::draw(screen);
    enemies.draw(screen);
    dying.draw(screen);
    bullets.draw(screen);
    for (std::map<std::string ,Group>::iterator it=items.begin();
        it != items.end(); ++it)
        it->second.draw(screen);
}

bool Level::collision_with(Sprite* sprite)
{
    if (sprite->is("Player"))
    {
        Sprite* enemy = enemies.first_sprite_colliding_with(sprite);
        Sprite* bullet = bullets.first_sprite_colliding_with(sprite);
        Sprite* fruit = items["Fruits"].first_sprite_colliding_with(sprite);
        Sprite* checkpoint = items["Checkpoints"].first_sprite_colliding_with(sprite);
        Sprite* box = items["Boxes"].first_sprite_colliding_with(sprite);
        if (enemy)
        {
            // Spikes out
            if (enemy->is("Turtle") and enemy->check_up)
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
                // l'enemie lui tombe dessus
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
                sprite->bump("box repulsion");
                box->bump();
                if (!items["Boxes"].has(box))
                {
                    std::string name[] = { "Apple", "Bananas", "Cherries", "Kiwi", "Melon", "Orange", "Pineapple", "Strawberry" };
                    int n(randint(1, 5));
                    for (int i=0; i<n; ++i)
                        items["Fruits"].add(new Fruit(box->get_x(), box->get_y()-5, this, name[rand()%8], true));
                }
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
    }
    else if (!sprite->is("Plant") and !sprite->is("Trunk"))
        if (enemies.has(sprite))
        {
            Sprite* bullet = bullets.first_sprite_colliding_with(sprite);
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

void Level::remove_enemy(Sprite* enemy)
{
    enemy->bump();
    enemies.remove(enemy);
    dying.add(enemy);
}
