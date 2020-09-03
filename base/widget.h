#ifndef WIDGET_H
#define WIDGET_H

<<<<<<< HEAD
#include <SDL/SDL.h>
#include "object.h"
#include "text.h"
#include "app.h"

// les fonctionnalit√©s d'un boutton sont encapsul√©s dans cette classe
=======
#include "object.h"
#include "text.h"
#include "app.h"
#include <SDL.h>

// les fonctionnalitÈs d'un boutton sont encapsulÈs dans cette classe
>>>>>>> 495eb918d6ae1f887b19d2afaa37aeac42acb24c
class __button__: public GameObject
{
public:
    virtual ~__button__();
    void set_position(int, int);
    void set_size(float);
    void set_text(const std::string&);
    void draw(SDL_Surface*);
    void update();

protected:
    enum { HOVER, CLICKED, IDLE };
    int state;
    SDL_Surface* background[3];
    Text* innerText;

    void define_bg(SDL_Surface* hover = NULL, SDL_Surface* clicked = NULL, SDL_Surface* idle = NULL);
    virtual void action() = 0;
};

class Button: public __button__
{
public:
    Button(const std::string&);
protected:
    virtual void action();
};

bool confirm(const std::string&);

class Confirm: public GameObject
{
public:
    Confirm(const std::string&);
    ~Confirm();
    void draw(SDL_Surface*);
    void update();
    int result;
private:
    class Ok: public Button
    {
    public :
        Ok(Confirm* c): Button("Annuler"), parent(c)
        {
            set_text("OK");
        }
    protected:
        void action()
        {
            parent->result = true;
            parent->kill();
        }
        Confirm* parent;
    };
    class Cancel: public Button
    {
    public:
        Cancel(Confirm* c): Button("Annuler"), parent(c) {}
    protected:
        void action()
        {
            parent->result = false;
            parent->kill();
        }
        Confirm *parent;
    };
    Text *message;
    Button *ok, *cancel;
};

/*
class TextArea: public GameObject
{
public:
    TextArea(SDL_Rect);
    void draw(SDL_Surface*);

private:
    Text* innerText;
    SDL_Surface* background;
};
*/
#endif // WIDGET_H
