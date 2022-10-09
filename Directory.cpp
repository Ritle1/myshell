/* NOTE-> This implementation file is for windows only. */

#include <iostream>
#include <string>
#include <Windows.h>

#include "Directory.h"

using namespace std;

const int MAX_PATH_LONG = 65535;

string Directory::getDir()
{
	return dir;
}

void Directory::changeDir(string mod)
{
	if (mod == "~"){ resetDir(); }
	else if (mod == "..") // Go back one directory if possible.
	{
		string::size_type pos = dir.find_last_of("\\/");
		dir = dir.substr(0, pos);
		cout << dir << endl;
	}
	
	else if (mod == "/") // Go back directorys to the root driver directory.
	{
		string::size_type pos = dir.find_first_of("\\/");
		dir = dir.substr(0, pos);
	}
	else if (DirectoryExists((dir + '\\' + mod).c_str())){
		dir += '\\' + mod;
	}
	else {
		cout << "Path does not exist.\n" << endl;
	}
}

void Directory::resetDir() // Please check pointer syntax is correct - Probably doesn't work!
{
	char buffer[MAX_PATH_LONG];
	GetModuleFileNameA(NULL, buffer, MAX_PATH_LONG);  // Getting the path into the buffer
	string::size_type pos = string(buffer).find_last_of("\\/");  // Removing empty memory in path.
	
	dir = string(buffer).substr(0, pos);
}

Directory::Directory()
{
	resetDir();
}
// Checks if a directory exists. (ASCII only - unicode support missing)
bool Directory::DirectoryExists(LPCSTR szPath)
{
  DWORD dwAttrib = GetFileAttributesA(szPath);  // Get[...]'A' means ASCII

  return (dwAttrib != INVALID_FILE_ATTRIBUTES && 
         (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}