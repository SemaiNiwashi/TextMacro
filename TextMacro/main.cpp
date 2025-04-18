#include "include\dirent.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <string>
#include <fstream>
#include <functional>
#include <algorithm>
#include <vector>
#include <direct.h>
#define GetCurrentDir _getcwd
using namespace std;

ifstream inputStream;
ofstream outputStream;

bool immediateInput = true;

void getLines(vector<string>&);
string deCapitalize(const string&);
//int checkLineEndForCommands(const string&, bool);
void writeNewCopy(string, const vector<string>&);
void fillVectorFromString(vector <string>&, const string&);
void getDirectory(string, vector<string>&, int);

bool isInt(string input)
{
	try
	{
		stoi(input);
	}
	catch (exception& err)
	{
		return false;
	}
	return true;
}

void getLines(vector<string>& lineGot)
{
	//Get all lines from file
	string temp;
	int myi = 0;
	while (!inputStream.eof())
	{
		getline(inputStream, temp);
		lineGot.push_back(temp);
	}
}

void fillVectorFromString(vector <string>& vectorList, const string& wordString)
{
	int wordBeg;
	int wordEnd = -1;

	while (true)
	{
		wordBeg = wordEnd + 1;
		wordEnd = wordString.find(' ', wordBeg);
		if (wordEnd == string::npos)
		{
			vectorList.push_back(wordString.substr(wordBeg, wordString.length() - wordBeg));
			break;
		}
		vectorList.push_back(wordString.substr(wordBeg, wordEnd - wordBeg));
	}
}

string deCapitalize(const string& line)
{
	string editLine = line;
	int capPos = 0;
	capPos = editLine.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ", capPos);
	while (capPos != string::npos)
	{
		editLine[capPos] = tolower(editLine[capPos]);
		//editLine[capPos] = char(int(editLine[capPos] + 32)); //Different way of doing same thing, no apparent difference in speed.
		capPos = editLine.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ", capPos);
	}
	return editLine;
}
/*
int checkLineEndForCommands(const string& line, bool checkStart)
{
	if (checkStart)
	{
		if (line[0] == '\\') //line starts with a '\'
		{
			int nextBreak;
			nextBreak = line.find_first_of(" :;!,./'\"/");
			if (nextBreak != string::npos)
			{
				return nextBreak;
			}
			else
			{
				if (debugging)
					cout << "Warning: From start, line consists only of commands. Line: \"" << line << "\"\n";
				return -1;
			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		if (line.find_last_of(wordBreaker) != string::npos && line[line.find_last_of(wordBreaker)] == '\\') //if the first wordbreaker back from the end is a '\'
		{
			int lastbreak;
			lastbreak = line.find_last_of(" :;!,./'\"/");
			if (lastbreak != string::npos)
			{
				lastbreak = line.find_first_of("\\", lastbreak);
				return lastbreak;
			}
			else
			{
				if (debugging)
					cout << "Warning: From end, line consists only of commands. Line: \"" << line << "\"\n";
				return -1;
			}
		}
		else
		{
			return line.length();
		}
	}
}
*/

//MACRO CODE HERE!!!
void TextMacro(vector<string>& lineGot)
{
	vector<string> lineGotOld;
	for (int i = 0; i < lineGot.size(); i++)
	{
		lineGotOld.push_back(lineGot[i]);
	}
	lineGot.clear();
	for (int i = 0; i < lineGotOld.size(); i++)
	{
		//---MACRO CODE HERE!!!-----------------
		//Example: in first 25 characters, find all text after 2nd ':', replace with "[Personal Memo]".
		int firstInd = lineGotOld[i].find_first_of(':',0);
		if (firstInd != string::npos) 
		{
			int secondInd = lineGotOld[i].find_first_of(':', firstInd+1);
			if ((secondInd == string::npos || secondInd  > 25) && firstInd < 25)
			{
				secondInd = firstInd;
			}
			if (secondInd != string::npos)
			{
				lineGotOld[i] = lineGotOld[i].substr(0, secondInd+1) + " [Personal Memo]\\par";
			}
		}
		//---MACRO CODE END-----------------

		if (lineGotOld[i] != "" && lineGotOld[i] != ".")
		{
			lineGot.push_back(lineGotOld[i]);
		}
	}
}

//other macro code
void TextMacroBudget(vector<string>& lineGot)
{
	string date;
	string amount;
	string description;
	int hasAll = 0;
	vector<string> lineGotOld;
	for (int i = 0; i < lineGot.size(); i++)
	{
		lineGotOld.push_back(lineGot[i]);
	}
	lineGot.clear();
	for (int i = 0; i < lineGotOld.size(); i++)
	{
		//MACRO CODE
		if (lineGotOld[i].substr(0, 3).find(' ') == string::npos && lineGotOld[i][3] == ' ' && (int(lineGotOld[i][4]) >= 48 && int(lineGotOld[i][4]) <= 57))
		{
			if (hasAll != 0)
			{
				std::cout << "Error! i:" << i << " hasAll: " << hasAll << "line: " << lineGotOld[i] << "\n";
			}
			//std::cout << lineGotOld[12] << "";
			int tabloc = lineGotOld[i].find('\t');
			date = lineGotOld[i].substr(0, tabloc);
			description = lineGotOld[i].substr(tabloc + 1, lineGotOld[i].length() - (tabloc + 1));
			hasAll = 2;
		}
		if (hasAll == 2)
		{
			if (lineGotOld[i][0] == '-' && lineGotOld[i][1] == '$')
			{
				amount = lineGotOld[i].substr(1, lineGotOld[i].find('\t') - 1);
				hasAll++;
			}
			else if (lineGotOld[i][0] == '$')
			{
				hasAll = 0;
			}
		}
		if (hasAll == 3)
		{
			hasAll = 0;
			lineGot.push_back(description + "\t\t\tMeridian Line of Credit\t" + date + '\t' + amount);
		}
	}
}

void writeNewCopy(string filePath, const vector<string>& lineGot)
{
	//Write results (to file)
	outputStream.open(filePath, ios::out);
	
	for (int i = 0; i < int(lineGot.size()); i++)
	{
		outputStream << lineGot[i] << "\n";
	}
	outputStream.close();
}

void getDirectory(string searchDirectory, vector<string>& itemsFound, int filtercode)
{
	//filtercode 0 = all, filtercode 1 = folders, filtercode 2 = files 
	unsigned char isFolder = 0x4;
	unsigned char isFile = 0x8;
	itemsFound.clear();
	DIR *dp;
	struct dirent *dirp;
	if ((dp = opendir(searchDirectory.c_str())) == NULL) {
		cout << "Error(" << errno << ") opening " << searchDirectory << endl;
	}

	while ((dirp = readdir(dp)) != NULL) {
		if (filtercode == 0 || (filtercode == 1 && dirp->d_type == 16384) || (filtercode == 2 && dirp->d_type == 32768))
		{
			if (!(dirp->d_type == 16384 && dirp->d_namlen <= 2))//Cut out all folders with 2-or-less character long names, because two mystery
			{													//folders called "." and ".." appear that do not evaluate to that as strings.
				itemsFound.push_back(string(dirp->d_name));
			}
		}
	}
	closedir(dp);
}

int main()
{
	char cCurrentPath[FILENAME_MAX];
	ofstream outputStream;
	string directory;
	string fileNamePt1;
	string userInput;
	vector<string> directoryItems;
	bool whileBool = true;
	//Start with program running directory
	if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
	{
		return errno;
	}
	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; //not really required
	directory = cCurrentPath;
	while (whileBool) {
		system("cls");
		getDirectory(directory, directoryItems, 1);
		if (int(directoryItems.size()) == 0)
		{
			userInput = "s";
		}
		else
		{
			cout << "(b) Up a folder      (s) Choose file in this folder\n";
			cout << "Directory: " << directory << "\n\n";
			for (int i = 0; i < int(directoryItems.size()); i++)
			{
				cout << "(" << i + 1 << ") " << directoryItems[i] << endl;
			}
			if (immediateInput)
			{
				userInput = _getch();
			}
			else
			{
				cin >> userInput;
			}
		}
		if (userInput == "b")
		{
			directory = directory.substr(0, directory.find_last_of("\\"));
		}
		else if (userInput == "s")
		{
			whileBool = false;
		}
		else if (isInt(userInput) && (stoi(userInput) - 1) < int(directoryItems.size()))
		{
			directory += "\\" + directoryItems[stoi(userInput) - 1];
		}
		else
		{
			cout << "\nThat is not a valid input. Please try again.\n";
			Sleep(1500);
		}
	}

	fileNamePt1 = directory.substr(directory.find_last_of("\\") + 1, directory.length() - (directory.find_last_of("\\") + 1));
	directory += "\\";
	
	//Find files
	getDirectory(directory, directoryItems, 2);
	vector<int> tempvec;
	whileBool = true;
	while (whileBool) {
		system("cls");
		cout << "Which file should we read?\n\n";
		for (int i = 0; i < int(directoryItems.size()); i++)
		{
			cout << "(" << i + 1 << ") " << directoryItems[i] << endl;
		}
		if (immediateInput && int(directoryItems.size()) < 10)
		{
			userInput = _getch();
		}
		else
		{
			cin >> userInput;
		}
		if (isInt(userInput) && (stoi(userInput) - 1) < int(directoryItems.size()))
		{
			fileNamePt1 = directoryItems[stoi(userInput) - 1];
			whileBool = false;
		}
		else
		{
			cout << "\nThat is not a valid input. Please try again.\n";
			Sleep(1500);
		}
	}

	system("cls");
	vector<string> lineGot;
	inputStream.open(directory + fileNamePt1, ios::out);
	cout << "Getting text...\n";
	getLines(lineGot);
	inputStream.close();
	TextMacro(lineGot);
	cout << "Writing Macro'd copy...\n";
	writeNewCopy(directory + "Macroed " + fileNamePt1, lineGot);
	cout << "Finished.\n";
	system("pause");
	return 0;
}
