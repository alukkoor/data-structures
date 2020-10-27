#ifndef Board_h
#define Board_h

#include "Side.h"
#include <cstdlib>

class Board
{
public:
    Board(int nHoles, int nInitialBeansPerHole);
        // Construct a Board with the indicated number of holes per side (not counting the pot) and initial number of beans per hole. If nHoles is not positive, act as if it were 1; if nInitialBeansPerHole is negative, act as if it were 0.
    Board(const Board& other);
        // Copy constructor
    Board& operator =( const Board& rhs);
        // Assignment operator
    ~Board();
        // Delete dynamically allocated arrays
    int holes() const;
        // Return the number of holes on a side (not counting the pot).
    int beans(Side s, int hole) const;
       // Return the number of beans in the indicated hole or pot, or −1 if the hole number is invalid.
    int beansInPlay(Side s) const;
        // Return the total number of beans in all the holes on the indicated side, not counting the beans in the pot.
    int totalBeans() const;
        //Return the total number of beans in the game, including any in the pots.
    bool sow(Side s, int hole, Side& endSide, int& endHole);
        //If the hole indicated by (s,hole) is empty or invalid or a pot, this function returns false without changing anything. Otherwise, it will return true after sowing the beans: the beans are removed from hole (s,hole) and sown counterclockwise, including s's pot if encountered, but skipping s's opponent's pot. The parameters endSide and endHole are set to the side and hole where the last bean was placed. (This function does not make captures or multiple turns; different Kalah variants have different rules about these issues, so dealing with them should not be the responsibility of the Board class.)
    bool moveToPot(Side s, int hole, Side potOwner);
        //If the indicated hole is invalid or a pot, return false without changing anything. Otherwise, move all the beans in hole (s,hole) into the pot belonging to potOwner and return true.
    bool setBeans(Side s, int hole, int beans);
        //If the indicated hole is invalid or beans is negative, this function returns false without changing anything. Otherwise, it will return true after setting the number of beans in the indicated hole or pot to the value of the third parameter. (This may change what beansInPlay and totalBeans return if they are called later.) This function exists solely so that we and you can more easily test your program: None of your code that implements the member functions of any class is allowed to call this function directly or indirectly. (We'll show an example of its use below.)
private:
    void swap(Board& other);
        // Swap contents of board with other
    int m_nHoles;
    int m_nInitial;
    int* m_side1;   // North holes
    int* m_side2;   // South holes
};

#endif /* Board_h */