#include "Player.h"
#include <iostream>
#include <cstdlib>
using namespace std;

Player::Player(std::string name) : m_name(name)
//Create a Player with the indicated name.
{}

std::string Player::name() const
//Return the name of the player.
{
    return m_name;
}

bool Player::isInteractive() const
// Return false if the player is a computer player. Return true if the player is human. Most kinds of players will be computer players.
{
    return false;
}

Player::~Player()
//Since this class is designed as a base class, it should have a virtual destructor.
{}


HumanPlayer::HumanPlayer(std::string name) : Player(name)
{}

bool HumanPlayer::isInteractive() const
{
    return true;
}

int HumanPlayer::chooseMove(const Board& b, Side s) const
// Prompt user to input moves
{
    int move;
    cout << "Select a hole, " << name() << ": ";
    cin >> move;
    while( move <= 0 || move > b.holes() || b.beans(s, move) == 0 )
    {
        if( move <= 0 || move > b.holes() )
            cout << "The hole number must be from 1 to " << b.holes() << "." << endl;
        else
            cout << "There are no beans in that hole." << endl;
        cout << "Select a hole, " << name() << ": ";
        cin >> move;
    }
    return move;
}


BadPlayer::BadPlayer(std::string name) : Player(name)
{}

int BadPlayer::chooseMove(const Board& b, Side s) const
// Return farthest valid move choice from own pot
{
    switch(s)
    {
        case NORTH:
            for( int i = b.holes(); i >= 1; i-- )
            {
                if( b.beans(s, i) != 0 )
                    return i;
            }
            return -1;
        case SOUTH:
            for( int i = 1; i <= b.holes(); i++ )
            {
                if( b.beans(s, i) != 0 )
                    return i;
            }
            return -1;
        default:
            exit(1);
    }
}


SmartPlayer::SmartPlayer(std::string name) : Player(name)
{}

int SmartPlayer::chooseMove(const Board& b, Side s) const
// Return best available move
{
    AlarmClock ac(4900);  // time out after 4.9 seconds
    int bestHole, value(-1000000), depth(5);
    bestMove(b, s, s, bestHole, value, depth);
    ac.timedOut();
    return bestHole;
}

void SmartPlayer::bestMove(Board b, Side s, Side sWantToWin, int& bestHole, int& value, int depth) const
// Find best available move and value given search depth
{
    
    // Stop if no valid moves
    bool over, hasWinner;
    Side winner;
    status(b, over, hasWinner, winner);
    if( over )
    {
        bestHole = -1;
        value = evaluation(b, sWantToWin);
        return;
    }
      
    // Terminating criterion
    if(depth < 1)
    {
        bestHole = -1;
        value = evaluation(b, sWantToWin);
        return;
    }
    
    // Check each hole
    Side es;
    int eh;
    for(int hole = 1; hole <= b.holes(); hole++)
    {
        // Make move
        Board temp(b);
        if( !b.sow(s, hole, es, eh ) )
           continue;
        
        if( s == NORTH && es == NORTH && eh != 0 && b.beans(NORTH, eh) == 1 && b.beans(SOUTH, eh) != 0 )
        {
            b.moveToPot(NORTH, eh, NORTH);
            b.moveToPot(SOUTH, eh, NORTH);
        }
        if( s == SOUTH && es == SOUTH && eh != 0 && b.beans(SOUTH, eh) == 1 && b.beans(NORTH, eh) != 0 )
        {
            b.moveToPot(NORTH, eh, SOUTH);
            b.moveToPot(SOUTH, eh, SOUTH);
        }
        
        // Recursive call
        int hole2, value2;
        if(eh != 0 )
            bestMove(b, opponent(s), sWantToWin, hole2, value2, depth-1);
        else
            bestMove(b, s, sWantToWin, hole2, value2, depth-1);
        
        // Unmake move
        Board b = temp;
        
        // Update best move
        if(value2 > value)
        {
            bestHole = hole;
            value = value2;
        }
    }
    return;
}

bool SmartPlayer::validMove(const Board& b, Side s) const
// Check if selecting hole is valid move
{
    for( int i = 1; i <= b.holes(); i++ )
    {
        if( b.beans(s, i) != 0 )
            return true;
    }
    return false;
}

void SmartPlayer::status(const Board& b, bool& over, bool& hasWinner, Side& winner) const
// Check if game is over 
{
    if( b.beansInPlay(NORTH) != 0 && b.beansInPlay(SOUTH) != 0 )
    {
        over = false;
        return;
    }
    over = true;
    if( b.beans(NORTH, 0) == b.beans(SOUTH, 0) )
        hasWinner = false;
    else
    {
        hasWinner = true;
        if( b.beans(NORTH, 0) > b.beans(SOUTH, 0) )
            winner = NORTH;
        else
            winner = SOUTH;
    }
}

int SmartPlayer::evaluation(const Board &b, Side sWantToWin) const
// Evaluate position on tree based on heuristics
{
    bool over, hasWinner;
    Side winner;
    status(b, over, hasWinner, winner);
    int value;
    if(over && hasWinner)
    {
        if(winner == sWantToWin)
            value = 10000;
        else
            value = -10000;
    }
    else if(over && !hasWinner)
        value = 0;
    else
        value = b.beans(sWantToWin, 0) + b.beansInPlay(sWantToWin) - b.beans(opponent(sWantToWin), 0) - b.beansInPlay(opponent(sWantToWin));
    return value;
}
