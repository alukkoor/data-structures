#include "Game.h"
#include "Player.h"
#include "Board.h"
#include "Side.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    Board b(6,4);
    SmartPlayer sp("Comp");
    BadPlayer bp("Homer");
    Game g(b, &bp, &sp);
    g.play();
}
