#ifndef Game_h
#define Game_h

#include "Board.h"
#include "Side.h"
#include "Player.h"

class Game
{
public:
    Game(const Board& b, Player* south, Player* north);
    void display() const;
    void status(bool& over, bool& hasWinner, Side& winner) const;
    bool move();
    void play();
    int beans(Side s, int hole) const;
    
private:
    Player* m_south;
    Player* m_north;
    Board m_board;
    bool m_northTurn;
};

#endif /* Game_h */
