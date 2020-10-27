#ifndef Player_h
#define Player_h

#include "Side.h"
#include "Board.h"
#include <string>
#include <chrono>
#include <future>
#include <atomic>

class Player
{
public:
    Player(std::string name);
    std::string name() const;
    virtual bool isInteractive() const;
    virtual int chooseMove(const Board& b, Side s) const = 0;
    //Every concrete class derived from this class must implement this function so that if the player were to be playing side s and had to make a move given board b, the function returns the move the player would choose. If no move is possible, return −1.
    virtual ~Player();
private:
    std::string m_name;
};

// Chooses its move by prompting a person running the program for a move
class HumanPlayer : public Player
{
public:
    HumanPlayer(std::string name);
    bool isInteractive() const;
    int chooseMove(const Board& b, Side s) const;
};

// Computer player that chooses an arbitrary valid move
class BadPlayer : public Player
{
public:
    BadPlayer(std::string name);
    int chooseMove(const Board& b, Side s) const;
};

// For any game played on a board of up to six holes per side, with up to four initial beans per hole, SmartPlayer::chooseMove must return its choice in no more than five seconds.
class SmartPlayer : public Player
{
public:
    SmartPlayer(std::string name);
    int chooseMove(const Board& b, Side s) const;
private:
    void bestMove(Board b, Side s, Side sWantToWin, int& bestHole, int& value, int depth) const;
    bool validMove(const Board& b, Side s) const;
    void status(const Board& b, bool& over, bool& hasWinner, Side& winner) const;
    int evaluation(const Board& b, Side s) const;
};

class AlarmClock // Set an alarm clock that will time out after the indicated number of ms
{
public:
    AlarmClock(int ms)
    {
        m_timedOut = false;
        m_isRunning = true;
        m_alarmClockFuture = std::async([=]() {
            for (int k = 0; k < ms  &&  m_isRunning; k++)
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            if (m_isRunning)
                m_timedOut = true;
        });
    }
    
    ~AlarmClock()
    {
        m_isRunning = false;
        m_alarmClockFuture.get();
    }
    
    bool timedOut() const
    // Will be false until the alarm clock times out; after that, always returns true.
    {
        return m_timedOut;
    }
    
    AlarmClock(const AlarmClock&) = delete;
    AlarmClock& operator=(const AlarmClock&) = delete;
private:
    std::atomic<bool> m_isRunning;
    std::atomic<bool> m_timedOut;
    std::future<void> m_alarmClockFuture;
};

#endif /* Player_h */
