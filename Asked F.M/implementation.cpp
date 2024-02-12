#pragma once
#include "Classes_Header.h"
#include "Utility.cpp"
int User::Counter = 1;
int Question::Counter = 1;

User::User()
{
}

User::User(vector<string> Data)
{
	if (Data[0] == "-1")
		ID = Counter++;
	else
	{
		ID = stoi(Data[0]);
		if (Counter <= ID)
			Counter = ID + 1;
	}

	user_name = Data[1];
	password = Data[2];
	name = Data[3];
	email = Data[4];
	anonymous = stoi(Data[5]);
}

pair<int, bool> User::getID() const
{
	return { ID,anonymous };
}

string User::getUsername() const
{
	return user_name;
}

string User::getData() const
{
	string line;
	line += itos(ID) + char(1);
	line += user_name + char(1);
	line += password + char(1);
	line += name + char(1);
	line += email + char(1);
	line += itos(anonymous) + char(1);

	return line;
}

bool User::exist(string userName_, string password_) const
{
	return (user_name == userName_ && password == password_);
}

void User::print() const
{
	cout << "ID: " << ID << "\tUser Name: " << user_name << '\n';
}

// -----------------------------------------------------------------------------------

Question::Question(int sender, int receiver)
{
	text = answer = "D.E";
	ID = Counter++;
	senderID = sender;
	receiverID = receiver;
	cout << "Enter question text: ";
	getline(cin, text);
	getline(cin, text);
}

Question::Question(vector<string> Data)
{
	ID = stoi(Data[0]);
	senderID = stoi(Data[1]);
	receiverID = stoi(Data[2]);
	text = Data[3];
	answer = Data[4];
	if (Counter <= ID)
		Counter = ID + 1;
	for (int i = 5; i < Data.size(); ++i)
		threads.push_back(stoi(Data[i]));
}

int Question::getSenderID() const
{
	return senderID;
}

int Question::getReceiverID() const
{
	return receiverID;
}

int Question::getID() const
{
	return ID;
}

string Question::getData() const
{
	string line;
	line += (itos(ID) + char(1));
	line += (itos(senderID) + char(1));
	line += (itos(receiverID) + char(1));
	line += (text + char(1));
	line += (answer + char(1));
	for (auto& thread : threads)
		line += (itos(thread) + char(1));
	return line;
}

vector<int> Question::getThreads() const
{
	return threads;
}

void Question::setAnswer()
{
	if (answer != "D.E")
		cout << "Warning: Already answered. Answer will be updated.\n";
	cout << "Enter answer: ";
	getline(cin, answer);
	getline(cin, answer);
}

void Question::setThread(int thread)
{
	threads.push_back(thread);
}

void Question::print(bool From, bool To) const
{
	cout << "Question ID(" << ID << ")";
	if (From)
		cout << " from user id(" << senderID << ")";
	if (To)
		cout <<  " to user id(" << receiverID << ")";
	
	cout << endl;
	cout << "\tQustion: " << text << '\n';
	cout << "\tAnswer: " << answer << '\n';
}

// -----------------------------------------------------------------------------------

void Manager::loadUsers()
{
	vector<string> lines = ReadFile("users.txt");
	for (string line : lines)
	{
		vector<string> Data = Split(line);
		User new_user(Data);
		id_user.push_back(new_user);
	}
}

void Manager::SaveUser()
{
	vector<string> lines;
	for (int user = 1; user < id_user.size();++user)
	{
		string line = id_user[user].getData();
		lines.push_back(line);
	}
	WriteFile(lines, "users.txt");
}

void Manager::loadQuestion()
{
	vector<string> lines = ReadFile("questions.txt");
	for (string line : lines)
	{
		vector<string> Data = Split(line);
		Question new_question(Data);
		int IDQ = new_question.getID();
		
		int sender = new_question.getSenderID();
		int receiver = new_question.getReceiverID();

		id_sender[sender].insert(IDQ);
		id_receiver[receiver].insert(IDQ);
		id_qustion.insert({ IDQ, new_question });
	}
}

void Manager::SaveQuestion()
{
	vector<string> lines;

	for (auto& user : id_receiver)
		for (auto& Q : user)
			lines.push_back(id_qustion[Q].getData());
	WriteFile(lines, "questions.txt");
}

Manager::Manager()
{
	userName = "";
	ID = -1;
	id_user.resize(1);
	loadUsers();

	id_sender.resize(id_user.size() + 1);
	id_receiver.resize(id_user.size() + 1);
	loadQuestion();
}

void Manager::aboutMe()const
{
	cout << "Username: " << userName << "\tID: " << (ID == -1 ? "" : itos(ID)) << '\n';
}

void Manager::Login()
{
	while (true)
	{
		cout << "Enter user name & password: ";
		string password;
		cin >> userName >> password;
		for (auto& user : id_user)
		{
			if(user.exist(userName, password))
			{
				ID = user.getID().first;
				return;
			}
		}
		cout << "user name & password invalid. Try again.\n";
	}
}

void Manager::SignUp()
{
	vector<string> Data(6);
	Data[0] = "-1";
	cout << "Enter user name. (No spaces): "; 
	cin >> userName;
	Data[1] = userName;
	cout << "Enter password: "; cin >> Data[2];
	cout << "Enter name: "; getline(cin, Data[3]);
	getline(cin, Data[3]);
	cout << "Enter email: "; cin >> Data[4];
	cout << "Enter anonymous questions?: (0 or 1) "; cin >> Data[5];
	User new_user(Data);
	ID = new_user.getID().first;
	id_user.push_back(new_user);
	
	id_sender.push_back({});
	id_receiver.push_back({});
	SaveUser();
}

void Manager::PrintToMe() const         // reciever
{
	unordered_set<int> printed;
	for (const int& IDQ : id_receiver[ID])
	{
		if (printed.count(IDQ))
			continue;

		bool From = id_user[id_qustion.at(IDQ).getSenderID()].getID().second;
		id_qustion.at(IDQ).print(!From, 0);
		vector<int> threads = id_qustion.at(IDQ).getThreads();
		for (int& thread : threads)
		{
			cout << "\n\tThread: ";
			bool From = id_user[id_qustion.at(IDQ).getReceiverID()].getID().second;
			id_qustion.at(thread).print(From, 0);
			printed.insert(thread);
		}
	}
}

void Manager::PrintFromMe() const        // sender
{
	unordered_set<int> printed;
	for (const int& IDQ : id_sender[ID])
	{
		if (printed.count(IDQ))
			continue;
		vector<int> threads = id_qustion.at(IDQ).getThreads();
		id_qustion.at(IDQ).print(0, 1);
		for (int& thread : threads)
		{
			cout << "\n\tThread: ";
			id_qustion.at(thread).print(0, 1);
			printed.insert(thread);
		}
	}
}

void Manager::Aswer()
{
	int IDQ;
	cout << "Enter Question id or -1 to cancel: ";
	cin >> IDQ;
	if (IDQ == -1)
		return;

	if (id_receiver[ID].count(IDQ) == 0)   // not found
	{
		cout << "You don't have access to answer this question.\n";
		return;
	}
	else
	{
		bool From = id_user[id_qustion.at(IDQ).getSenderID()].getID().second;
		id_qustion.at(IDQ).print(!From, 0);
		id_qustion.at(IDQ).setAnswer();
		SaveQuestion();
	}
}

void Manager::Delete()
{
	cout << "Enter Quetion id or -1 to cancel: ";
	int IDQ; cin >> IDQ;
	if (IDQ == -1)
		return;
	if (id_sender[ID].count(IDQ) == 0)   // not found
	{
		cout << "You don't have access to delete this question.\n";
		return;
	}
	else
	{
		vector<int> threads = id_qustion[IDQ].getThreads();
		for (int& threadQ : threads)
		{
			id_sender[ID].erase(threadQ);
			id_receiver[id_qustion.at(threadQ).getReceiverID()].erase(threadQ);
			id_qustion.erase(threadQ);
		}

		id_sender[ID].erase(IDQ);
		id_receiver[id_qustion.at(IDQ).getReceiverID()].erase(IDQ);
		id_qustion.erase(IDQ);

		SaveQuestion();
	}
}

void Manager::Ask()
{
	int receiverID;
	cout << "Enter user id or -1 to cancel: ";
	cin >> receiverID;
	if (receiverID == -1)
		return;

	int threadID;
	cout << "For thread question: Enter Question id or -1 for new question: ";
	cin >> threadID;
	if (threadID != -1 && id_qustion.count(threadID) == 0)
	{
		cout << "There is no question with this ID.\n";
		return;
	}
	
	Question new_question(ID, receiverID);
	int IDQ = new_question.getID();
	if (threadID != -1)
		id_qustion.at(threadID).setThread(IDQ);
	
	id_qustion.insert({IDQ, new_question });
	id_sender[ID].insert(IDQ);
	id_receiver[receiverID].insert(IDQ);
	
	SaveQuestion();
}

void Manager::List_Users() const
{
	for (int i = 1; i < id_user.size(); ++i)
		id_user[i].print();
}

void Manager::logout()
{
	userName = "";
	ID = -1;
}

void Manager::Feed() const
{
	cout << "Asked F.M contain " << id_qustion.size() << " questions.\n\n";
	unordered_set<int> printed;
	for (auto& IDQ : id_qustion)
	{
		if (printed.count(IDQ.first))
			continue;

		bool From = id_user[id_qustion.at(IDQ.first).getSenderID()].getID().second;
		IDQ.second.print(!From, 1);
		vector<int> threads = id_qustion.at(IDQ.first).getThreads();
		for (int& thread : threads)
		{
			cout << "\n\tThread: ";
			bool From = id_user[id_qustion.at(IDQ.first).getReceiverID()].getID().second;
			id_qustion.at(thread).print(From, 1);
			printed.insert(thread);
		}

	}
}

// -----------------------------------------------------------------------------------