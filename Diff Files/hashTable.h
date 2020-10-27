#ifndef hashTable_h
#define hashTable_h

#include <utility> // for std::pair
#include <string>
#include <vector>

class HashTable
{
public:
    HashTable();
    void insert(const std::string &s, const int &spot);
    bool search(const std::string &s, int &spot);
private:
    static const int nBuckets = 9997;
    std::vector< std::pair<std::string, int> > table[nBuckets];
    unsigned long hasher(const std::string &s);
};

#endif /* hashTable_h */
