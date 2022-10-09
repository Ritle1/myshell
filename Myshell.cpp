#include <iostream>
#include <string>
#include <vector>
#include "Directory.h"
#include "Commands.h"

using namespace std;

const string WHITESPACE = " \n\r\t\f\v";

// Error constants
const string MISSING_ARGS = "Sorry, Command arguments are incorrect.\n\n";

Commands shell = Commands();
void inputLoop();
string lower(string x);
string trim(const std::string &s);
string rtrim(const std::string &s);
string ltrim(const std::string &s);
vector<string> split(string x);

int main()
{
	cout << "---------------------\n\nMyShell v1 - c++ Windows edition.\n" << endl;
	inputLoop();
}

void inputLoop()
{
	bool quit = false;
	while (!quit)
	{
		cout << "~myshell " << shell.pwd() << "\\> ";
		string command;
		getline(cin, command);
		vector<string> args = split(command);
		command = lower(args.at(0));
		if (command == "exit" || command == "quit")
			goto exit;
		
		else if (command == "pwd")
			cout << shell.pwd() << "\n\n";
		
		else if (command == "cd"){
			if (args.size() > 1)
				shell.cd(args.at(1));
			else
				cout << MISSING_ARGS;
		}
		
		else if (command == "ls" || command == "dir")
			(args.size() > 1) ? shell.ls(args.at(1)) : shell.ls("");
			
		
		else
			cout << "Command: '" << command << "' is unkonwn.\n" << endl;
	}
	exit:
	return;
}


void pipeManager()
{
	// Manage command pipes.
}

//----------------------------------------
//          String Modifications         |
//----------------------------------------

// Returning a new string but lowercased. (argument is by value)
string lower(string x){
		for (size_t i = 0; i < x.length(); i++)
		{
			x[i] = tolower(x[i]);
		}
		return x;
}

vector<string> split(string x)
{
	trim(x);
	size_t spaceCounter  = 0;
	for(size_t i =0; i < x.length(); i++){
		if (x[i] == ' ')
			spaceCounter++;
	}
	
	vector<string> ret(0); // Initial vector is empty.

	if (spaceCounter == 0){
		ret.push_back(x);
		return ret;
	}
	
	for(size_t i = 0; i < spaceCounter; i++){
		size_t spaceIndex = x.find(' ');
		ret.push_back(x.substr(0, spaceIndex));
		x = x.substr(spaceIndex + 1);
	}
	ret.push_back(x);
	return ret;
}

string ltrim(const std::string &s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}
 
string rtrim(const std::string &s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}
 
string trim(const std::string &s) {
    return rtrim(ltrim(s));
}