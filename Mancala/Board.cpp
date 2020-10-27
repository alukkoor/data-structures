#include "Board.h"

Board::Board(int nHoles, int nInitialBeansPerHole)
// Construct a Board with the indicated number of holes per side (not counting the pot) and initial number of beans per hole. If nHoles is not positive, act as if it were 1; if nInitialBeansPerHole is negative, act as if it were 0.
{
    if( nHoles <= 0 )
        m_nHoles = 1;
    else
        m_nHoles = nHoles;
    
    if( nInitialBeansPerHole < 0 )
        m_nInitial = 0;
    else
        m_nInitial = nInitialBeansPerHole;
    
    // Dynamically allocate arrays for holes
    m_side1 = new int[m_nHoles+1];
    m_side2 = new int[m_nHoles+1];
    m_side1[0] = 0;
    m_side2[0] = 0;
    for( int i = 1; i <= nHoles; i++ )
    {
        m_side1[i] = m_nInitial;
        m_side2[i] = m_nInitial;
    }
}

Board::Board(const Board& other)
// Copy constructor
{
    m_nHoles = other.m_nHoles;
    m_nInitial = other.m_nInitial;
    m_side1 = new int[m_nHoles+1];
    m_side2 = new int[m_nHoles+1];
    for( int i = 0; i <= m_nHoles; i++ )
    {
        m_side1[i] = other.m_side1[i];
        m_side2[i] = other.m_side2[i];
    }
}

Board& Board::operator =( const Board& rhs )
// Assignment operator
{
    Board temp(rhs);
    swap(temp);
    return *this;
}

Board::~Board()
// Delete dynamically allocated arrays
{
    delete[] m_side1;
    delete[] m_side2;
}

int Board::holes() const
// Return the number of holes on a side (not counting the pot).
{
    return m_nHoles;
}

int Board::beans(Side s, int hole) const
// Return the number of beans in the indicated hole or pot, or −1 if the hole number is invalid.
{
    if( hole < 0 || hole > m_nHoles )
        return -1;
    switch(s)
    {
        case NORTH:
            return m_side1[hole];
        case SOUTH:
            return m_side2[hole];
        default:
            exit(1);
    }
}

int Board::beansInPlay(Side s) const
// Return the total number of beans in all the holes on the indicated side, not counting the beans in the pot.
{
    int count = 0;
    switch(s)
    {
        case NORTH:
            for( int i = 1; i <= m_nHoles; i++ )
                count += m_side1[i];
            break;
        case SOUTH:
            for( int i = 1; i <= m_nHoles; i++ )
                count += m_side2[i];
            break;
        default:
            exit(1);
    }
    return count;
}

int Board::totalBeans() const
//Return the total number of beans in the game, including any in the pots.
{
    int count = 0;
    
    for( int i = 0; i <= m_nHoles; i++ )
        count += m_side1[i];
    for( int i = 0; i <= m_nHoles; i++ )
        count += m_side2[i];
    return count;
}

bool Board::sow(Side s, int hole, Side& endSide, int& endHole)
//If the hole indicated by (s,hole) is empty or invalid or a pot, this function returns false without changing anything. Otherwise, it will return true after sowing the beans: the beans are removed from hole (s,hole) and sown counterclockwise, including s's pot if encountered, but skipping s's opponent's pot. The parameters endSide and endHole are set to the side and hole where the last bean was placed. (This function does not make captures or multiple turns; different Kalah variants have different rules about these issues, so dealing with them should not be the responsibility of the Board class.)
{
    if( hole == 0 || hole > m_nHoles || (beans(s, hole) == 0) )
        return false;
    switch(s)
    {
        case NORTH:
            while( (beans(s, hole) != 0) )
            {
                for( int i = hole-1; i >= 0; i-- )
                {
                    if( (beans(s, hole) == 0) )
                        break;
                    m_side1[i] += 1;
                    m_side1[hole] -= 1;
                    endSide = NORTH;
                    endHole = i;
                }
                for( int i = 1; i <= m_nHoles; i++ )
                {
                    if( (beans(s, hole) == 0) )
                        break;
                    m_side2[i] += 1;
                    m_side1[hole] -= 1;
                    endSide = SOUTH;
                    endHole = i;
                }
                for( int i = m_nHoles; i > hole; i-- )
                {
                    if( (beans(s, hole) == 0) )
                        break;
                    m_side1[i] += 1;
                    m_side1[hole] -= 1;
                    endSide = NORTH;
                    endHole = i;
                }
            }
            break;
        case SOUTH:
            while( (beans(s, hole) != 0) )
            {
                for( int i = hole+1; i <= m_nHoles; i++ )
                {
                    if( (beans(s, hole) == 0) )
                        break;
                    m_side2[i] += 1;
                    m_side2[hole] -= 1;
                    endSide = SOUTH;
                    endHole = i;
                }
                if( (beans(s, hole) != 0) )
                {
                    m_side2[0] += 1;
                    m_side2[hole] -= 1;
                    endSide = SOUTH;
                    endHole = 0;
                }
                for( int i = m_nHoles; i > 0; i-- )
                {
                    if( (beans(s, hole) == 0) )
                        break;
                    m_side1[i] += 1;
                    m_side2[hole] -= 1;
                    endSide = NORTH;
                    endHole = i;
                }
                for( int i = 0; i < hole; i++ )
                {
                    if( (beans(s, hole) == 0) )
                        break;
                    m_side2[i] += 1;
                    m_side2[hole] -= 1;
                    endSide = SOUTH;
                    endHole = i;
                }
            }
            break;
        default:
            exit(1);
    }
    return true;
}

bool Board::moveToPot(Side s, int hole, Side potOwner)
//If the indicated hole is invalid or a pot, return false without changing anything. Otherwise, move all the beans in hole (s,hole) into the pot belonging to potOwner and return true.
{
    if( hole > m_nHoles || hole == 0 )
        return false;
    if( potOwner != NORTH && potOwner != SOUTH )
        exit(1);
    int move;
    switch(s)
    {
        case NORTH:
            move = m_side1[hole];
            if( potOwner == NORTH )
                m_side1[0] += move;
            else
                m_side2[0] += move;
            m_side1[hole] = 0;
            break;
        case SOUTH:
            move = m_side2[hole];
            if( potOwner == NORTH )
                m_side1[0] += move;
            else
                m_side2[0] += move;
            m_side2[hole] = 0;
            break;
        default:
            exit(1);
    }
    return true;
}

bool Board::setBeans(Side s, int hole, int beans)
//If the indicated hole is invalid or beans is negative, this function returns false without changing anything. Otherwise, it will return true after setting the number of beans in the indicated hole or pot to the value of the third parameter. (This may change what beansInPlay and totalBeans return if they are called later.) This function exists solely so that we and you can more easily test your program: None of your code that implements the member functions of any class is allowed to call this function directly or indirectly. (We'll show an example of its use below.)
{
    if( hole < 0 || hole > m_nHoles || beans < 0 )
        return false;
    switch(s)
    {
        case NORTH:
            m_side1[hole] = beans;
            break;
        case SOUTH:
            m_side2[hole] = beans;
            break;
        default:
            exit(1);
    }
    return true;
}

void Board::swap(Board& other)
// Swap contents of board with other
{
    int* ptr1 = other.m_side1;
    other.m_side1 = m_side1;
    m_side1 = ptr1;
    
    int* ptr2 = other.m_side2;
    other.m_side2 = m_side2;
    m_side2 = ptr2;
    
    int tempNHoles = other.m_nHoles;
    other.m_nHoles = m_nHoles;
    m_nHoles = tempNHoles;
    
    int tempNInitial = other.m_nInitial;
    other.m_nInitial = m_nInitial;
    m_nInitial = tempNInitial;
}
