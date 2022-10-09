#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

class Directory
{
	private:
		string dir = "error->unassigned>> "; // The current directory is saved as a string.
		bool DirectoryExists(LPCSTR szPath);
		
	public:
		string getDir();
		// Modifiying the current directory.
		void changeDir(string mod);
		// reset directory to the executable's directory.
		void resetDir();
		// Constructracting the directory (will call resetDir)
		Directory();
	
};


#endif
