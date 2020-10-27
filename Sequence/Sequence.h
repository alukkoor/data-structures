#ifndef Sequence_h
#define Sequence_h

#include <string>
using ItemType = std::string;

class Sequence
{
public:
    // Constructors/Operator/Destructor
    Sequence();
    Sequence(const Sequence& other );
    Sequence& operator =( const Sequence& rhs);
    ~Sequence();
    
    // Accessors
    bool empty() const;
    int size() const;
    void dump() const;
    bool get(int pos, ItemType& value) const;
    int find(const ItemType& value) const;
    
    // Mutators
    int insert(int pos, const ItemType& value);
    int insert(const ItemType& value);
    bool erase(int pos);
    int remove(const ItemType& value);
    void swap(Sequence& other);
    bool set(int pos, const ItemType& value);
    
private:
    // Data Members
    struct Node
    {
        ItemType m_data;
        Node* m_next;
        Node* m_prev;
    };
    Node* m_head;
    int m_length;
};

// Non-member functions
int subsequence( const Sequence& seq1, const Sequence& seq2 );
void interleave( const Sequence& seq1, const Sequence& seq2, Sequence& result );

#endif /* Sequence_h */

Triangle& Triangle::operator=(const Triangle &t)
{
    if(*this != t)
    {
        delete [] p;
        p = new Point[3];
        for(int i=0;i<3;i++)
            p[i] = t.p[i];
    }
    return *this;
}
