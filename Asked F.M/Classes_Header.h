#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>

using namespace std;

class Question
{
private:
	static int Counter;
	int ID;
	int senderID, receiverID;
	string text;
	string answer;
	vector<int> threads;
public:
	Question(int sender = -1, int receiver = -1);
	Question(vector<string> Data);
	int getID()const;
	void setAnswer();
	void setThread(int thread);
	int getSenderID()const;
	int getReceiverID()const;
	string getData()const;
	vector<int> getThreads()const;
	void print(bool From, bool To)const;
};

class User
{
private:
	string user_name;
	string password;
	string name;
	string email;
	bool anonymous;
	int ID;
	static int Counter;
public:
	User();
	User(vector<string> Data);
	void print()const;
	pair<int, bool> getID()const;
	string getUsername()const;
	string getData()const;
	bool exist(string userName_, string password_)const;
};

class Manager
{
private:
	string userName;
	int ID;
	vector<User> id_user;
	vector<set<int>> id_sender, id_receiver;
	map<int, Question> id_qustion;

	void loadUsers();
	void loadQuestion();
	void SaveUser();
	void SaveQuestion();
public:
	Manager();
	void aboutMe()const;
	void Login();
	void SignUp();
	void List_Users()const;
	void Ask();
	void Feed()const;
	void logout();
	void PrintFromMe()const;
	void PrintToMe()const;
	void Aswer();
	void Delete();
};