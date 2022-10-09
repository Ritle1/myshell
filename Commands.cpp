#include <iostream>
#include <string>
#include <windows.h>
#include <filesystem>
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
void Commands::ls(string flags){
	// Determening flags
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
				cout << tmp_path << ", ";
				continue;
			}
			// else printing each file in it's own line with detailed information.
			string line = "---- ";
			line[0] = (attributes & FILE_ATTRIBUTE_DIRECTORY) ? 'd' : '-';
			line[1] = (attributes & FILE_READ_ACCESS || attributes & FILE_ALL_ACCESS) ? 'r' : '-';
			line[2] = (attributes & FILE_WRITE_ACCESS || attributes & FILE_ALL_ACCESS) ? 'w' : '-';
			line[3] = (attributes & FILE_EXECUTE) ? 'x' : '-';
			line += tmp_path.substr(tmp_path.find_last_of("\\/") + 1);
			cout << line << endl;
		}
	}
	cout << ((as_list) ? "\n" : "\n\n");
	return;
}  
 
void Commands::mkdir(string path){return;}  // Make a directory.
void Commands::rm(string path){return;} // Delete a directory or file.
void Commands::mv(string sPath, string ePath){return;} // Rename or/and Repath.
void Commands::cp(string sPath, string ePath){return;} // Create a duplicate of a file.
void Commands::cat(string path){return;} // concatenate i.e show file contents on screen. (as text)
void Commands::echo(string x){return;} // show x on screen. (as text)
void Commands::touch(string path){return;} // create a file on a certain path.

// Constructor
Commands::Commands(){
	dir = Directory();
}
