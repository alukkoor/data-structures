#include "Sequence.h"
#include <iostream>

using namespace std;

Sequence::Sequence() : m_length(0)   // Constructor: create dummy node
{
    m_head = new Node;
    m_head->m_next = m_head;
    m_head->m_prev = m_head;
}

Sequence::Sequence( const Sequence& other ) // Copy constructor
{
    m_length = 0;
    m_head = new Node;
    m_head->m_next = m_head;
    m_head->m_prev = m_head;
    Node* p = other.m_head->m_next;
    while( p != other.m_head )
    {
        insert( m_length, p->m_data );
        p = p->m_next;
    }
}

Sequence& Sequence::operator =( const Sequence& rhs ) // Assignment operator
{
    Sequence temp( rhs );
    swap( temp );
    return *this;
}

Sequence::~Sequence() // Delete all nodes
{
     while( m_head->m_next != m_head )
    {
        Node* ptr = m_head->m_prev;
        m_head->m_prev = ptr->m_prev;
        ptr->m_prev->m_next = m_head;
        delete ptr;
    }
     delete m_head;
}

bool Sequence::empty() const  // Return true if the sequence is empty, otherwise false.
{
    if ( m_length == 0 )
        return true;
    else
        return false;
}

int Sequence::size() const    // Return the number of items in the sequence.
{
    return m_length;
}

void Sequence::dump() const     // Print list for debugging
{
    Node* p = m_head->m_next;
    while( p != m_head )
    {
        cerr << p->m_data << endl;
        p = p->m_next;
    }
}

int Sequence::insert(int pos, const ItemType& value) // Insert value into sequence at position pos
{
    if( pos < 0 || pos > m_length )
        return(-1);
    Node* ptr = m_head->m_next;       // Access correct index
    for( int i = 0; i < pos; i++ )
        ptr = ptr->m_next;
    Node* ins = new Node;           // Insert new node
    ins->m_data = value;
    ins->m_prev = ptr->m_prev;
    ins->m_next = ptr;
    ptr->m_prev = ins;
    ins->m_prev->m_next = ins;
    m_length++;
    return pos;
}

int Sequence::insert(const ItemType& value) // Insert value at first pos where value <= item or end
{
    int pos = m_length;
    Node* ptr = m_head->m_next;       // Access correct index
    for( int i = 0; ptr != m_head; i++ )
    {
        if( value <= ptr->m_data)     // Check insert condition
        {
            pos = i;
            break;
        }
        ptr = ptr->m_next;
    }
    Node* ins = new Node;           // Insert new node
    ins->m_data = value;
    ins->m_prev = ptr->m_prev;
    ins->m_next = ptr;
    ptr->m_prev = ins;
    ins->m_prev->m_next = ins;
    m_length++;
    return pos;
}

bool Sequence::erase(int pos) // Remove element at pos
{
    if ( pos < 0 || pos >= m_length )
        return false;
    Node* ptr = m_head->m_next;
    for( int i = 0; i < pos; i++ )       // Find index
        ptr = ptr->m_next;
    ptr->m_next->m_prev = ptr->m_prev;   // Remove node
    ptr->m_prev->m_next = ptr->m_next;
    delete ptr;
    m_length--;
    return true;
}

int Sequence::remove(const ItemType& value) // Remove all elts that == value
{
    int count = 0;
    Node* ptr = m_head->m_next;
    for( int i = 0; ptr != m_head; i++ )
    {
        if( value == ptr->m_data)     // Check insert condition
        {
            Node* temp = ptr;
            ptr = ptr->m_prev;
            temp->m_next->m_prev = temp->m_prev;      // Remove node(s)
            temp->m_prev->m_next = temp->m_next;
            delete temp;
            m_length--;
            count++;
        }
        ptr = ptr->m_next;
    }
    return( count );
}

bool Sequence::get(int pos, ItemType& value) const // Copy item at pos into value
{
    if ( pos < 0 || pos >= m_length )
        return false;
    Node* ptr = m_head->m_next;
    for( int i = 0; i < pos; i++ )       // Find index
        ptr = ptr->m_next;
    value = ptr->m_data;                 // Get value
    return true;
}

bool Sequence::set(int pos, const ItemType& value) // Copy value into item at pos
{
    if( pos < 0 || pos >= m_length )
        return false;
    Node* ptr = m_head->m_next;
    for( int i = 0; i < pos; i++ )       // Find index
        ptr = ptr->m_next;
    ptr->m_data = value;                // Set value
    return true;
}

int Sequence::find(const ItemType& value) const // Find smallest index with item == value
{
    Node* ptr = m_head->m_next;
    for( int i = 0; ptr != m_head; i++ )
    {
        if( value == ptr->m_data)     // Check insert condition
        {
            return i;                 // Return index
        }
        ptr = ptr->m_next;
    }
    return -1;
}

void Sequence::swap(Sequence& other) // Exchange contents of this sequence with other
{
    Node* ptr = other.m_head;           // Swap m_head
    other.m_head = m_head;
    m_head = ptr;
    
    int tempLength = other.m_length;    // Swap length
    other.m_length = m_length;
    m_length = tempLength;
}

int subsequence( const Sequence& seq1, const Sequence& seq2 )
{
    if ( seq1.size() < seq2.size() || seq2.empty() )
        return -1;
    ItemType value1, value2;
    for( int i = 0; i < seq1.size(); i++ )
    {
        seq1.get(i, value1);       // Check for match with seq2 first digit
        seq2.get(0, value2);
        if( value1 == value2 )
        {
            int temp = i;
            for ( int j = 0; j < seq2.size(); j++ )       // Check for match with subsequence
            {
                seq1.get(temp, value1);
                seq2.get(j, value2);
                if( value1 != value2 )
                    break;
                if( j == seq2.size() - 1 )
                    return i;
                temp++;
                if( temp == seq1.size() )
                    return -1;
            }
        }
    }
    return -1;
}

void interleave( const Sequence& seq1, const Sequence& seq2, Sequence& result )
{
    Sequence temp;             // Alias sequence
    int tempIndex(0), seq1Index(0), seq2Index(0);
    ItemType value;
    while( seq1Index < seq1.size() || seq2Index < seq2.size() )
    {
        if( seq1.get( seq1Index, value ) )        // get values of seq1 and insert into result
        {
            temp.insert( tempIndex, value );
            seq1Index++;
            tempIndex++;
        }
        if( seq2.get( seq2Index, value) )        // get values of seq1 and insert into result
        {
            temp.insert( tempIndex, value );
            seq2Index++;
            tempIndex++;
        }
    }
    result = temp;             // Set result to alias
}
