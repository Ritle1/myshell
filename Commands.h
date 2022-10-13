#ifndef COMMANDS_H
#define COMMANDS_H

#include "Directory.h"

class Commands
{
	private:
		Directory dir;
	public:
		void cd(string mod); // change directory path.
		string pwd(); // Get current working directory.
		string ls(string flags); // list all files in a directory.
		void mkdir(string path);  // Make a directory.
		void rm(string path); // Delete a directory or file.
		void mv(string sPath, string ePath); // Rename or/and change path.
		void cp(string sPath, string ePath); // Create a duplicate of a file.
		string cat(string path); // concatenate i.e show file contents on screen. (as text)
		void echo(string x); // show x on screen. (as text)
		void touch(string path); // create a file on a certain path.
		void clear(); // Clear screen of any characters.
		bool process(string path); // Create a subprocess.
		void write(string data, string path); // Write data into a file.
		void help(); // returns a list of all commands + usage.
		
		// Constructor
		Commands();
};


#endif
