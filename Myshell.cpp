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
vector<string> split(string x, char split_by);
struct commandResult{
	bool stateCode; // true if there is a return value, false if there isn't.
	string retValue;  // the function return value (if exists)
};
commandResult commandHandler(string command);
void pipeManager(string input);
void print(string x);

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
		cout << "~myshell " << trim(shell.pwd()) << "\\> ";
		string command;
		getline(cin, command);
		pipeManager(command);
	}
	exit:
	return;
}


void pipeManager(string input)
{
	// Manage command pipes.
	vector<string> commands = split(input, '|');
	commandResult nextValue;
	nextValue.stateCode = false; // No argumnet to pass on first command.
	int cnt = 0;
	for (string command : commands){
		cnt ++;
		if (nextValue.stateCode){
			command = trim(command); // Trimming command without args
			command += " " + nextValue.retValue;
			nextValue = commandHandler(command);
			}
		else{
			nextValue = commandHandler(command);
		}
			
	}
	// Checking if last command has any output - and printing ->
	if (nextValue.stateCode)
			(nextValue.retValue.find('\n') == string::npos) 
			? print(nextValue.retValue + "\n\n") 
			: print(nextValue.retValue);
}

commandResult commandHandler(string command){
	command = trim(command); // Removing spaces at the start & beggining.

	vector<string> args = split(command, ' ');
	commandResult res;
	command = lower(args.at(0));
	if (command == "exit" || command == "quit")
		exit(0); // Program exit at code 0, no error, no piping.
	else if (command == "pwd"){
		res.retValue = shell.pwd();
		res.stateCode = true;
	}
	else if (command == "cd"){
		(args.size() > 1) ? shell.cd(args.at(1)) : print(MISSING_ARGS);
		res.stateCode = false;
	}
	else if (command == "ls" || command == "dir"){
		(args.size() > 1) ? res.retValue = shell.ls(args.at(1)) : res.retValue = shell.ls("");
		res.stateCode = true;
	}
	else if (command == "mkdir"){
		(args.size() > 1) ? shell.mkdir(args[1]) : print(MISSING_ARGS);
		res.stateCode = false;
	}
	else if (command == "rm"){
		(args.size() > 1) ? shell.rm(args[1]) : print(MISSING_ARGS);
		res.stateCode = false;
	}
	else if (command == "mv"){
		(args.size() > 2) ? shell.mv(args[1], args[2]) : print(MISSING_ARGS);
		res.stateCode = false;
	}
	else if (command == "cp"){
		(args.size() > 2) ? shell.cp(args[1], args[2]) : print(MISSING_ARGS);
		res.stateCode = false;
	}
	else if (command == "echo"){
		(args.size() > 1) ? shell.echo(args[1]) : shell.echo("");
		res.stateCode = false;
	}
	else if (command == "cat"){
		if (args.size() > 1){
			res.retValue = shell.cat(args[1]);
			res.stateCode = true;
		}
		else{
			print(MISSING_ARGS);
			res.stateCode = false;
		}
		
	}
	else if (command == "touch"){
		(args.size() > 1) ? shell.touch(args[1]) : print(MISSING_ARGS);
		res.stateCode = false;
	}
	else if (command == "clear" || command == "cls"){
		shell.clear();
		res.stateCode = false;
	}
	else if (command != ""){
		cout << "Command: '" << command << "' is unkonwn.\n" << endl;
		res.stateCode = false;
	}
	return res;
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


vector<string> split(string x, char split_by)
{
	trim(x);
	size_t spaceCounter  = 0;
	for(size_t i =0; i < x.length(); i++){
		if (x[i] == split_by)
			spaceCounter++;
	}
	
	vector<string> ret(0); // Initial vector is empty.

	if (spaceCounter == 0){
		ret.push_back(x);
		return ret;
	}
	
	for(size_t i = 0; i < spaceCounter; i++){
		size_t spaceIndex = x.find(split_by);
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
	string s2 = s;
	for (auto & c : s2)
		if(int(c) == -1){
		c = ' ';}
    return rtrim(ltrim(s2));
}

void print(string x){
	cout << x;
}