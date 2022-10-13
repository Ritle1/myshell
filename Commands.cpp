#include <iostream>
#include <string>
#include <windows.h>
#include <filesystem>
#include <fstream> // for cat
#include <stdlib.h> // For clear or cls
#include "Commands.h"
#include "Directory.h"


// change directory path.
void Commands::cd(string mod){
	dir.changeDir(mod);
	return;
}

//Get current working directory.
string Commands::pwd(){
	return dir.getDir();
}  

// list files in the current directory i.e. dir.getDir().
string Commands::ls(string flags){
	// Determening flags
	string ret = ""; // test output to return.
	bool all = false;
	bool as_list = false;
	if (flags.size() > 0 && flags[0] == '-'){
		if(flags.find('a') != string::npos)
			all = true;
		if(flags.find('l') != string::npos)
			as_list = true;
	}
	// Printing files according to flags(if any)
	for (const auto & file : filesystem::directory_iterator(pwd() + "/")){
		string tmp_path = file.path().string();
		// Show the file if all is true, or if it isn't hidden.
		DWORD attributes = GetFileAttributesA(file.path().string().c_str());
		if(all || !(attributes & FILE_ATTRIBUTE_HIDDEN))
		{
			if (!as_list){
				tmp_path = tmp_path.substr(tmp_path.find_last_of("\\/") + 1);
				ret += tmp_path + ", ";
				continue;
			}
			// else printing each file in it's own line with detailed information.
			string line = "---- ";
			line[0] = (attributes & FILE_ATTRIBUTE_DIRECTORY) ? 'd' : '-';
			line[1] = (attributes & FILE_READ_ACCESS || attributes & FILE_ALL_ACCESS) ? 'r' : '-';
			line[2] = (attributes & FILE_WRITE_ACCESS || attributes & FILE_ALL_ACCESS) ? 'w' : '-';
			line[3] = (attributes & FILE_EXECUTE) ? 'x' : '-';
			line += tmp_path.substr(tmp_path.find_last_of("\\/") + 1);
			ret += line + "\n";
		}
	}
	ret += ((as_list) ? "\n" : "\n\n");
	return ret;
}  

// Make a directory.
void Commands::mkdir(string path){
	bool result = false;
	// Full path is specified (D:\some\path)
	if (path.find(':') != string::npos) 
		result = CreateDirectoryA(path.c_str(), NULL);
	// use current working path instead. (some\path)
	else{ 
		result = CreateDirectoryA((dir.getDir() + '\\' + path).c_str(), NULL);
	}
	// checking for errors 
	if (!result){
		cout << "Could not create the directory.\n";
		cout << "Error: " << GetLastError() << "\n\n";
	}
	return;
}  

// Delete a directory or file.
void Commands::rm(string path){
	bool result = false;
	// If full path is not specified, use the current working path.
	if(path.find(':') == string::npos)
		path = dir.getDir() + '\\' + path;
	// Deleting a directory
	if (GetFileAttributesA(path.c_str()) & FILE_ATTRIBUTE_DIRECTORY){
		result = RemoveDirectoryA(path.c_str());
	}
	// Deleting a file
	else {
		result = DeleteFileA(path.c_str());
	}
	// checking for errors 
	if (!result){
		cout << "Could not delete specified path.\n";
		cout << "Error: " << GetLastError() << "\n\n";
	}
	return;
} 

// Rename or/and Change path.
void Commands::mv(string sPath, string ePath){
	// If full path is not specified, use the current working path.
	if(sPath.find(':') == string::npos)
		sPath = dir.getDir() + '\\' + sPath;
	if(ePath.find(':') == string::npos) // same for the end path.
		ePath = dir.getDir() + '\\' + ePath;
	// Preforming the move/ rename action and checking for errors if exist.
	if(!MoveFileA(sPath.c_str(), ePath.c_str())){
		cout << "Could not move/rename the specified path.\n";
		cout << "Error: " << GetLastError() << "\n\n";
	}
	return;
} 

// Duplicate a file. (on a different path or name)
void Commands::cp(string sPath, string ePath){
	if(sPath.find(':') == string::npos)
		sPath = dir.getDir() + '\\' + sPath;
	if(ePath.find(':') == string::npos) // same for the end path.
		ePath = dir.getDir() + '\\' + ePath;
	// Preforming the copy action and checking for errors if exist.
	// (Will not override existing files)
	if(!CopyFileA(sPath.c_str(), ePath.c_str(), true)){
		cout << "Could not move/rename the specified path.\n";
		cout << "Error: " << GetLastError() << "\n\n";
	}
	return;
	} 

// concatenate i.e show file contents on screen. (as text)
string Commands::cat(string path){

	if (path.find('>') != string::npos) return "";

	// If full path is not specified, use the current working path.
	if(path.find(':') == string::npos)
		path = dir.getDir() + '\\' + path;

	fstream file;
	file.open(path, ios::in);
	if (!file) // Checking failbit flag state
		return "Error opening the file: " + path;
	string data = "";
	char c;
	while(true){
		c = file.get();
		data += c;
		if (file.eof()) // Checking if EndOfFile bit is on.
			break;
	}
	file.close(); // Done using the file
	return (data);
} 

// show x on screen. (as text)
void Commands::echo(string x){
	cout << x << "\n\n";
	return;
}

// create a file on a certain path.
void Commands::touch(string path){
	// If full path is not specified, use the current working path.
	if(path.find(':') == string::npos)
		path = dir.getDir() + '\\' + path;

	HANDLE h = CreateFileA(path.c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if(!h){
		cout << "Could not create the specified path/file.\n";
		cout << "Error: " << GetLastError() << "\n\n";
		return;
	}
	CloseHandle(h);
	return;
}

// Launch another process. (returns if succeded or failed) 
bool Commands::process(string path){
	// If full path is not specified, use the current working path.
	if(path.find(':') == string::npos)
		path = dir.getDir() + '\\' + path;
	STARTUPINFOA info={sizeof(info)};
	PROCESS_INFORMATION processInfo;
	// Note: Second parameter can be a command argument but is set to NULL.
	if (CreateProcessA(path.c_str(), NULL, NULL, NULL, TRUE, 0, NULL, NULL, &info, &processInfo))
	{
		// Wait for the process to finish.
    	WaitForSingleObject(processInfo.hProcess, INFINITE);  
    	CloseHandle(processInfo.hProcess);  // Close the handles
    	CloseHandle(processInfo.hThread);
		return true;
	}
	return false;
}

void Commands::write(string data, string path){
	// If full path is not specified, use the current working path.
	if(path.find(':') == string::npos)
		path = dir.getDir() + '\\' + path;	

	fstream file;
	file.open(path, ios::out);
	if(!file){  // Couldn't open the file.
		file.close();
		cout << "Couldn't write to file.\n\n";
		return;
	}
	file.write(data.c_str(), data.size());
	file.close(); // finished using the file.
}

// Clear screen
void Commands::clear(){
	system("CLS");
	cout << flush;
}

// Constructor
Commands::Commands(){
	dir = Directory();
}
