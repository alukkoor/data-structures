/*#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

enum CallType {
    VOICE, TEXT
};
*/
class Medium
{
public:
    Medium( string id ) { m_id = id; }
    virtual ~Medium() {}
    virtual string connect() const = 0;
    string id() const { return m_id; };
    virtual string transmit( string msg) const { return("text: " + msg); }
private:
    string m_id;
};

class TwitterAccount : public Medium
{
public:
    TwitterAccount( string id ) : Medium( id ) {}
    ~TwitterAccount() { cout << "Destroying the Twitter account " << id() << "." << endl; }
    string connect() const { return "Tweet"; }
};

class Phone : public Medium
{
public:
    Phone( string id, CallType type ) : Medium( id ), m_type(type) {}
    ~Phone() { cout << "Destroying the phone " << id() << "." << endl; }
    string connect() const { return "Call"; }
    string transmit( string msg ) const
    {
        switch( m_type )
        {
            case VOICE:
                return("voice: " + msg);
            case TEXT:
                return("text: " + msg);
            default:
                exit(1);
        }
    }
private:
    CallType m_type;
};

class EmailAccount : public Medium
{
public:
    EmailAccount( string id ) : Medium( id ) {}
    ~EmailAccount() { cout << "Destroying the email account " << id() << "." << endl; }
    string connect() const { return "Email"; }
};

void send(const Medium* m, string msg)
{
    cout << m->connect() << " using id " << m->id()
    << ", sending " << m->transmit(msg) << endl;
}
/*
int main()
{
    Medium* media[4];
    media[0] = new TwitterAccount("UCLABruinAlert");
    // Phone users may prefer messages to be left as voice or text.
    media[1] = new Phone("(310) 825 3894", VOICE);
    media[2] = new Phone("(213) 389 9060", TEXT);
    media[3] = new EmailAccount("jbroon@ucla.edu");
    
    cout << "Send the message." << endl;
    for (int k = 0; k < 4; k++)
        send(media[k], "Major power outage in West L.A.");
    
    // Clean up the media before exiting
    cout << "Cleaning up." << endl;
    for (int k = 0; k < 4; k++)
        delete media[k];
}
*/
