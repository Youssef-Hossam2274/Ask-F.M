#pragma once
#include <iostream>
#include <vector> 
#include <fstream>
#include <string>
using namespace std;

static vector<string> ReadFile(string fileName)
{
	vector<string> lines;
	fstream file_handle(fileName.c_str());
	string line;
	if (file_handle.fail())
	{
		cout << "there is some error in  ReadFile function.\n";
		return {};
	}
	while (getline(file_handle, line))
	{
		lines.push_back(line);
	}
	return lines;
}

static vector<string> Split(string line)
{
	string word;
	vector<string> words;
	for (int i = 0; i < line.size(); ++i)
	{
		word += line[i];
		if (line[i] == char(1))
		{
			word.pop_back();
			words.push_back(word);
			word.clear();
		}
	}
	return words;
}

static void WriteFile(vector<string> lines, string fileName)
{
	ofstream file_handle(fileName.c_str(), ios::in | ios::out | ios::trunc);
	if (file_handle.fail())
	{
		cout << "there some thing is error in WriteFile function.\n";
		return;
	}
	
	for (string& str : lines)
		file_handle << str << '\n';
}

static string itos(int num)
{
	string strNum;
	bool isNeg = false;
	if (num < 0)
		num *= -1, isNeg = true;
	if (num == 0)
		return "0";
	while (num)
	{
		int digit = num % 10;
		strNum = char(digit + '0') + strNum;
		num /= 10;
	}
	if (isNeg)
		strNum = '-' + strNum;
	return strNum;
}

static int display(vector<string> menu, int start, int end)
{
	cout << "Menu:\n";
	for (int i = 1; i <= menu.size() ;++i)
		cout << "\t" << i << ": " << menu[i - 1];

	int choice;
	cout << "Enter number in range " << start << " - " << end << ": ";
	cin >> choice;
	while (choice < start || choice > end)
	{
		cout << "invalid number. Try again.\n";
		cout << "Enter number in range " << start << " - " << end << ": ";
		cin >> choice;
	}
	return choice;
}