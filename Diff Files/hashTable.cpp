#include "hashTable.h"
#include <functional> // for std::hash
using namespace std;

HashTable::HashTable() {}

void HashTable::insert(const string &s, const int &spot)
{
    unsigned long bucket = hasher(s);
    pair <string, int> add;
    add = make_pair(s, spot);
    table[bucket].push_back(add);
}

bool HashTable::search(const string &s, int &spot)
{
    unsigned long bucket = hasher(s);
    for( auto ptr = table[bucket].begin(); ptr != table[bucket].end(); ptr++ )
    {
        if(ptr->first == s)
        {
            spot = ptr->second;
            return true;
        }
    }
    return false;
}

unsigned long HashTable::hasher(const string &s)
{
    hash<string> str_hash;
    unsigned long hashValue = str_hash(s);
    unsigned long bucketNum = hashValue % nBuckets;
    return bucketNum;
}
