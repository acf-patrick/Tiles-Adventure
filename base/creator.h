#ifndef CREATOR_H
#define CREATOR_H

#include <map>
#include <string>
#include "object.h"

/// \brief Basic function object for game object creation.
/// \brief Override this class and specify how to create your object
class ObjectCreator
{
public:
    virtual ~ObjectCreator();

    static void  addToParameters(void*, const std::string&);
    static void* getParameter(const std::string&);

    /// \param _x, _y : position where the object will be created
    /// \return The game object.
    virtual GameObject* operator()(int, int) = 0;

private:
    // parameters are stored here by ID
    static std::map<std::string, void*> params;
};

class ObjectFactory
{
public:
    static void destroy();

    static GameObject* create(const std::string&, int, int);

    static void registre_creator(const std::string&, ObjectCreator*);

private:
    static std::map<std::string, ObjectCreator*> creators;
};

#endif // CREATOR_H
