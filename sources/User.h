#pragma once
#include <string>
#include "sha1.h"
using namespace std;

class User
{
    static int userCounter;
    int _id;
    string _login;
    string _username;
    Hash _passwordHash;
    bool _banned; // To track whether the user is banned

public:
    User();
    User(string username, Hash passHash);

    // Existing methods
    int checklogin(const string& login, const Hash& passHash) const;
    string& getuserName();
    int getUserID() const;

    // New methods
    void setBanned(bool banned);  // Set ban status
    bool isBanned() const;        // Check if user is banned
    void disconnect();            // Disconnect the user
};
