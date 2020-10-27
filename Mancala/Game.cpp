#include "Game.h"
#include <iostream>
using namespace std;

Game::Game(const Board& b, Player* south, Player* north) : m_south(south), m_north(north), m_board(b), m_northTurn(false)
// Construct a Game to be played with the indicated players on a copy of the board b. The player on the south side always moves first.
{}

void Game::display() const
//Display the game's board in a manner of your choosing, provided you show the names of the players and a reasonable representation of the state of the board.
{
    for( int i = 1; i < m_board.holes()/2; i++ )
        cout << '\t';
    cout << m_north->name() << endl;
    cout << "  ";
    for( int i = 1; i <= m_board.holes(); i++ )
        cout << m_board.beans(NORTH, i) << "  ";
    cout << endl << m_board.beans(NORTH, 0);
    for( int i = 1; i < m_board.holes(); i++ )
        cout << '\t';
    cout << " ";
    cout << m_board.beans(SOUTH, 0) << endl << "  ";
    for( int i = 1; i <= m_board.holes(); i++ )
        cout << m_board.beans(SOUTH, i) << "  ";
    cout << endl;
    for( int i = 1; i < m_board.holes()/2; i++ )
        cout << '\t';
    cout << m_south->name() << endl;
}

void Game::status(bool& over, bool& hasWinner, Side& winner) const
//If the game isn't over (i.e., more moves are possible), set over to false and do not change anything else. Otherwise, set over to true and hasWinner to true if the game has a winner, or false if it resulted in a tie. If hasWinner is set to false, leave winner unchanged; otherwise, set it to the winning side.
{
    if( m_board.beansInPlay(NORTH) != 0 && m_board.beansInPlay(SOUTH) != 0 )
    {
        over = false;
        return;
    }
    over = true;
    if( m_board.beans(NORTH, 0) == m_board.beans(SOUTH, 0) )
        hasWinner = false;
    else
    {
        hasWinner = true;
        if( m_board.beans(NORTH, 0) > m_board.beans(SOUTH, 0) )
            winner = NORTH;
        else
            winner = SOUTH;
    }
}

bool Game::move()
//If the game is over, return false. Otherwise, make a complete move for the player whose turn it is (so that it becomes the other player's turn) and return true. "Complete" means that the player sows the seeds from a hole and takes any additional turns required or completes a capture. If the player gets an additional turn, you should display the board so someone looking at the screen can follow what's happening.
{
    bool over, hasWinner;
    Side winner;
    Side es = NORTH;
    int eh(0);
    status(over, hasWinner, winner);
    if(over)
        return false;
    
    // Extra turn if eh is pot
    while( !over )
    {
        if( m_northTurn )
        {
            int hole = m_north->chooseMove(m_board, NORTH);
            m_board.sow(NORTH, hole, es, eh);
            if( !m_north->isInteractive() )
                cout << m_north->name() << " chooses hole " << hole << "." << endl;
        }
        else
        {
            int hole = m_south->chooseMove(m_board, SOUTH);
            m_board.sow(SOUTH, hole, es, eh);
            if( !m_south->isInteractive() )
                cout << m_south->name() << " chooses hole " << hole << "." << endl;
        }
        if(eh != 0)
            break;
        status(over, hasWinner, winner);
        display();
        
        // Announce if human has second turn
        if( m_northTurn && m_north->isInteractive() )
            cout << m_north->name() << " gets another turn." << endl;
        if( !m_northTurn && m_south->isInteractive() )
            cout << m_south->name() << " gets another turn." << endl;
    }
    
    // Capture opponent's beans
    if( m_northTurn && es == NORTH && eh != 0 && m_board.beans(NORTH, eh) == 1 && m_board.beans(SOUTH, eh) != 0)
    {
        m_board.moveToPot(NORTH, eh, NORTH);
        m_board.moveToPot(SOUTH, eh, NORTH);
    }
    if( !m_northTurn && es == SOUTH && eh != 0 && m_board.beans(SOUTH, eh) == 1 && m_board.beans(NORTH, eh) != 0)
    {
        m_board.moveToPot(NORTH, eh, SOUTH);
        m_board.moveToPot(SOUTH, eh, SOUTH);
    }
    
    // End game
    status(over, hasWinner, winner);
    if( over )
    {
        for( int i = 1; i <= m_board.holes(); i++ )
        {
            m_board.moveToPot(NORTH, i, NORTH);
            m_board.moveToPot(SOUTH, i, SOUTH);
        }
    }
    
    // Switch turn
    if(m_northTurn)
        m_northTurn = false;
    else
        m_northTurn = true;
    return true;
}

void Game::play()
//Play the game. Display the progress of the game in a manner of your choosing, provided that someone looking at the screen can follow what's happening. If neither player is interactive, then to keep the display from quickly scrolling through the whole game, it would be reasonable periodically to prompt the viewer to press ENTER to continue and not proceed until ENTER is pressed. (The ignore function for input streams is useful here.) Announce the winner at the end of the game. You can apportion to your liking the responsibility for displaying the board between this function and the move function. (Note: If when this function is called, South has no beans in play, so can't make the first move, sweep any beans on the North side into North's pot and act as if the game is thus over.)
{
    bool over, hasWinner;
    Side winner;
    status(over, hasWinner, winner);
    display();
    
    // Check if already over
    if(over)
    {
        for( int i = 1; i <= m_board.holes(); i++ )
        {
            m_board.moveToPot(NORTH, i, NORTH);
            m_board.moveToPot(SOUTH, i, SOUTH);
        }
        display();
    }
    
    // Play game
    while(!over)
    {
        move();
        display();
        status(over, hasWinner, winner);
        if(over)
            break;
        if( !m_north->isInteractive() && !m_south->isInteractive() )
        {
            cout << "Press ENTER to continue";
            cin.ignore(10000,'\n');
        }
    }
    
    // Declare winner
    if( winner == NORTH )
        cout << "The winner is " << m_north->name() << "." << endl;
    else if( winner == SOUTH )
        cout << "The winner is " << m_south->name() << "." << endl;
    else
        cout << "Tie." << endl;
}

int Game::beans(Side s, int hole) const
//Return the number of beans in the indicated hole or pot of the game's board, or −1 if the hole number is invalid. This function exists so that we and you can more easily test your program.
{
    return m_board.beans(s, hole);
}
