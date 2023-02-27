#include "game.h"
#include <stdio.h>

int main()
{
game* g = new game() ;

g->display_grid();
g->run();
return 0;
}
