#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include "creator.h"

std::map<std::string, void*> ObjectCreator::params;

ObjectCreator::~ObjectCreator() {}

void ObjectCreator::addToParameters(void* param, const std::string& ID)
{
    params[ID] = param;
}
void* ObjectCreator::getParameter(const std::string& ID)
{
    if (!params[ID])
    {
        std::cerr << ID + " : is not an ID in parameters list!" << std::endl;
        exit(1);
    }
    return params[ID];
}

std::map<std::string, ObjectCreator*> ObjectFactory::creators;

void ObjectFactory::destroy()
{
    for (std::map<std::string, ObjectCreator*>::iterator it = creators.begin();
         it != creators.end(); ++it)
        delete it->second;
}
void ObjectFactory::registre_creator(const std::string& key, ObjectCreator* value)
{
    // memorize only if not yet in
    if (!creators[key])
        creators[key] = value;
    else
        delete value;
}
GameObject* ObjectFactory::create(const std::string& objectType, int _x, int _y)
{
    ObjectCreator* creator(creators[objectType]);
    if (!creator)
    {
        std::cerr << objectType << " : Ce type n'a aucun génerateur d'objet!" << std::endl;
        return NULL;
        // exit(1);
    }
    return (*creator)(_x, _y);
}
