#include <iostream>
#include <fstream>
#include <sstream>  // for istringstream and ostringstream
#include <string>
#include <cassert>
#include <algorithm>
#include <iterator>
#include "hashTable.h"
#include "utilities.h"
using namespace std;

void createDiff(istream& fold, istream& fnew, ostream& fdiff)
{
    // Write contents of files into strings
    char temp;
    string s1, s2;
    while ( fold.get(temp) )
        s1 += temp;
    while( fnew.get(temp) )
        s2 += temp;
    const int N = 16;
    
    // Store N-char strings in hash table
    HashTable h;
    int offset;
    for( int spot = 0; spot < s1.length(); spot++ )
    {
        string s1write = "";
        for( int i = 0; i + spot < s1.length() && i < N; i++ )
        {
            s1write += s1[i + spot];
        }
        if( s1write.length() == N && !h.search(s1write, offset) )
            h.insert(s1write, spot);
    }
    
    // Search hash table
    for( int j = 0; j < s2.length(); )
    {
        string s2write = "";
        for( int i = 0; i+j < s2.length() && i < N; i++ )
        {
            s2write += s2[i+j];
        }
        if( h.search(s2write, offset) )     // Copy function
        {
            int L = N;
            for(int k = offset+N; k < s1.length() && j+L < s2.length() ; k++)
            {
                if( s1[k] != s2[j+L] )
                    break;
                L++;
            }
            fdiff << "C" << L << "," << offset;
            j += L;
        }
        else        // Add function
        {
            int amt = 0;
            for( int spot = j; !h.search(s2write, offset) && spot < s2.length(); spot++ )
            {
                amt += 1;
                s2write = "";
                for( int k = 0; k+spot+1 < s2.length() && k < N; k++ )
                {
                    s2write += s2[k+spot+1];
                }
            }
            fdiff << "A" << amt << ":";
            for( int n = 0; n < amt; n++ )
                fdiff << s2[j+n];
            j += amt;
        }
    }
}

bool applyDiff(istream& fold, istream& fdiff, ostream& fnew)
{
    char temp;
    string sold;
    while ( fold.get(temp) )
        sold += temp;
    string sNew;
    char cmd;
    int length, offset;
    
    // Command actions
    getCommand(fdiff, cmd, length, offset);
    while( cmd != 'x' )
    {
        switch (cmd)
        {
            // Implement add
            case 'A':
                for( int i = 0; i < length; i++ )
                {
                    char add;
                    fdiff.get(add);
                    sNew += add;
                }
                break;
            // Implement copy
            case 'C':
                if( length + offset > sold.length() )
                    return false;
                for( int i = 0; i < length; i++)
                    sNew += sold[i+offset];
                break;
            case '\r':
            case '\n':
                break;
        }
        if( !getCommand(fdiff, cmd, length, offset) )
            return false;
    }
    fnew << sNew;
    return true;
}
/*
#include <iostream>
#include <sstream>  // for istringstream and ostringstream
#include <string>
#include <cassert>
using namespace std;

void runtest(string oldtext, string newtext)
{
    istringstream oldFile(oldtext);
    istringstream newFile(newtext);
    ostringstream diffFile;
    createDiff(oldFile, newFile, diffFile);
    string result = diffFile.str();
    cout << "The diff file length is " << result.size()
    << " and its text is " << endl;
    cout << result << endl;
    
    oldFile.clear();   // clear the end of file condition
    oldFile.seekg(0);  // reset back to beginning of the stream
    istringstream diffFile2(result);
    ostringstream newFile2;
    assert(applyDiff(oldFile, diffFile2, newFile2));
    //cout << newFile2.str() << endl;
    assert(newtext == newFile2.str());
    
}

int main()
{
    runtest("There's a bathroom on the right.",
            "There's a bad moon on the rise.");
    runtest("ABCDEFGHIJBLAHPQRSTUVPQRSTUV",
            "XYABCDEFGHIJBLETCHPQRSTUVPQRSTQQELF");
    cout << "All tests passed" << endl;
}

*/
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <cassert>
using namespace std;

bool runtest(string oldName, string newName, string diffName, string newName2)
{
    if (diffName == oldName  ||  diffName == newName  ||
        newName2 == oldName  ||  newName2 == diffName  ||
        newName2 == newName)
    {
        cerr << "Files used for output must have names distinct from other files" << endl;
        return false;
    }
    ifstream oldFile(oldName, ios::binary);
    if (!oldFile)
    {
        cerr << "Cannot open " << oldName << endl;
        return false;
    }
    ifstream newFile(newName, ios::binary);
    if (!newFile)
    {
        cerr << "Cannot open " << newName << endl;
        return false;
    }
    ofstream diffFile(diffName, ios::binary);
    if (!diffFile)
    {
        cerr << "Cannot create " << diffName << endl;
        return false;
    }
    createDiff(oldFile, newFile, diffFile);
    diffFile.close();
    
    oldFile.clear();   // clear the end of file condition
    oldFile.seekg(0);  // reset back to beginning of the file
    ifstream diffFile2(diffName, ios::binary);
    if (!diffFile2)
    {
        cerr << "Cannot read the " << diffName << " that was just created!" << endl;
        return false;
    }
    ofstream newFile2(newName2, ios::binary);
    if (!newFile2)
    {
        cerr << "Cannot create " << newName2 << endl;
        return false;
    }
    if(!applyDiff(oldFile, diffFile2, newFile2))
    {
        cerr << "applyDiff failed" << endl;
        return false;
    }
    newFile2.close();
    
    newFile.clear();
    newFile.seekg(0);
    ifstream newFile3(newName2, ios::binary);
    if (!newFile)
    {
        cerr << "Cannot open " << newName2 << endl;
        return false;
    }
    if ( ! equal(istreambuf_iterator<char>(newFile), istreambuf_iterator<char>(),
                 istreambuf_iterator<char>(newFile3), istreambuf_iterator<char>()))
    {
        
        cerr << newName2 << " is not identical to " << newName
        << "; test FAILED" << endl;
        return false;
    }
    return true;
}

int main()
{
    assert(runtest("/Users/amithlukkoor/Desktop/warandpeace1.txt", "/Users/amithlukkoor/Desktop/warandpeace2.txt", "/Users/amithlukkoor/Desktop/diff.txt", "/Users/amithlukkoor/Desktop/new.txt"));
    cerr << "Test PASSED" << endl;
}

