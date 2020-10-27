#include <stack>
#include <iostream>
using namespace std;

class Coord
{
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};

bool pathExists(char maze[][10], int sr, int sc, int er, int ec);

int main()
{
    char maze[10][10] = {
        { 'X','X','X','X','X','X','X','X','X','X' },
        { 'X','.','.','.','.','X','.','.','.','X' },
        { 'X','.','X','X','.','X','X','X','.','X' },
        { 'X','X','X','.','.','.','.','X','.','X' },
        { 'X','.','X','X','X','.','X','X','X','X' },
        { 'X','.','X','.','.','.','X','.','.','X' },
        { 'X','.','.','.','X','.','X','.','.','X' },
        { 'X','X','X','X','X','.','X','.','X','X' },
        { 'X','.','.','.','.','.','.','.','.','X' },
        { 'X','X','X','X','X','X','X','X','X','X' }
    };
    
    if (pathExists(maze, 3,5, 8,8))
        cout << "Yeah boi 2" << endl;
    else
        cout << "Out of luck!" << endl;
    
}

bool pathExists(char maze[][10], int sr, int sc, int er, int ec)
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
{
    stack<Coord> coordStack;
    Coord end(er,ec);
    coordStack.push( Coord(sr,sc) );
    maze[sr][sc] = '#';
    while( !coordStack.empty() )
    {
        Coord b = coordStack.top();
        coordStack.pop();
        if( b.r() == end.r() && b.c() == end.c() )
            return true;
        if( maze[b.r()+1][b.c()] == '.' ) // Check south
        {
            maze[b.r()+1][b.c()] = '#';
            coordStack.push( Coord( b.r()+1,b.c() ) );
        }
        if( maze[b.r()][b.c()-1] == '.' ) // Check west
        {
            maze[b.r()][b.c()-1] = '#';
            coordStack.push( Coord( b.r(),b.c()-1 ) );
        }
        if( maze[b.r()-1][b.c()] == '.' ) // Check north
        {
            maze[b.r()-1][b.c()] = '#';
            coordStack.push( Coord( b.r()-1,b.c() ) );
        }
        if( maze[b.r()][b.c()+1] == '.' ) // Check east
        {
            maze[b.r()][b.c()+1] = '#';
            coordStack.push( Coord( b.r(),b.c()+1 ) );
        }
    }
    return false;
}

