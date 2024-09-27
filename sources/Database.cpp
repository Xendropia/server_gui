#include "Database.h"
#include "Parsing.h"
#include "sha1.h"
#include <algorithm>
#include <memory>

//'Ανθρωπος

int Database::searchUserByName(string username)
{
	auto uit = _usersMapByName.find(username);
	if (uit != _usersMapByName.end()) return uit->second;
  return -1;
}

vector<string> Database::getUserList() const
{
  vector<string> userList;
  for(auto user : _usersMapByName)
  {
    userList.push_back(user.first);
  }
  return userList;
}

string Database::getUserName(int userId) const
{
  for (auto it = _usersMapByName.begin(); it != _usersMapByName.end(); ++it) {
    if (it->second == userId)
      return it->first;
  }
  return "";
}

Database::Database() : _users(), _messages()
{
}

int Database::addUser(string username, string password)
{
	if (!correctName(username)) return -1;
	auto uit = _usersMapByName.find(username);
	if (uit != _usersMapByName.end()) return -2;
	User newUser = User(username, sha1(password));
	_users.push_back(newUser);
  _usersMapByName.insert({ username, newUser.getUserID() });
	return newUser.getUserID();
}

int Database::checkPassword(string username, string password)
{
	int result = -1;
	Hash passHash = sha1(password);
	for (const auto &u : _users)
	{
		result = u.checklogin(username, passHash);
		if (result != -1) return result;
	}
	return result;
}

void Database::addChatMessage(string sender, string text)
{
	_messages.push_back(Message(sender, text));
}

bool Database::addPrivateMessage(string sender, string target, string message)
{
	int targetUser = searchUserByName(target);
	if (targetUser < 0)
	{
		return false;
	}
  _messages.push_back(Message(sender, targetUser, message));
	return true;
}

vector<string> Database::getChatMessages()
{
	vector<string> strings;
	for (auto &m: _messages)
	{
		if (m.getDest() == -1)
		{
			strings.push_back("<" + m.getSender() + ">: " + m.getText());
		}
	}
	return strings;
}

User* Database::getUserById(int userId)
{
    // Find the user by ID in the list of users (_users is the correct variable)
    auto it = std::find_if(_users.begin(), _users.end(), [userId](User& user) {
        return user.getUserID() == userId;
    });

    // Return pointer to the user if found, otherwise return nullptr
    if (it != _users.end()) {
        return &(*it);
    } else {
        return nullptr;
    }
}
void Database::banUser(int userId)
{
    // Logic to ban the user
    // For example, setting the user's status to "banned" in your user storage.
    auto user = getUserById(userId);
    if (user) {
        user->setBanned(true); // Assuming you have a 'setBanned' method in your user class.
    }
}

void Database::disconnectUser(int userId)
{
    // Logic to disconnect the user
    // For example, closing their active session or removing them from active connections.
    auto user = getUserById(userId);
    if (user) {
        user->disconnect(); // Assuming you have a 'disconnect' method in your user class.
    }
}

vector<Message> Database::getPrivateMessage(int userID)
{
	vector<Message> strings;
	//int userID = searchUserByName(username);
	for (auto &m : _messages)
  {
    if(userID == -1 && m.getDest() != -1)
      strings.push_back(m);
    else if(userID != -1 && m.getDest() == userID)
      strings.push_back(m);
	}
	return strings;
}
