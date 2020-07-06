#include <ctime>
#include "game.h"

int main(int argc, char** argv)
{
    srand(time(0));
    Game* g = new Game();
    g->run();
    delete g;
    return 0;
}
