/* NOTE-> This implementation file is for windows only. */

#include <iostream>
#include <string>
#include <Windows.h>

#include "Directory.h"

using namespace std;

const int MAX_PATH_LONG = 65535;

static bool allCharactersSame(string s);

string Directory::getDir()
{
	return dir;
}

void Directory::changeDir(string mod)
{
	// 1) stay on the same directory if it's all dots & length is not 2. (in that case go back a directory)
	if(mod.find('.') != string::npos && mod.length() != 2 && allCharactersSame(mod)){}
	// 2) same but for slashes - which have a use only when using one.
	else if((mod.find('/') != string::npos || mod.find('\\') != string::npos) && mod.length() != 1 && allCharactersSame(mod)){}
	else if (mod == "~"){ resetDir(); }
	else if (mod == "..") // Go back one directory if possible.
	{
		string::size_type pos = dir.find_last_of("\\/");
		dir = dir.substr(0, pos);
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

static bool allCharactersSame(string s)
{
    int n = s.length();
    for (int i = 1; i < n; i++)
        if (s[i] != s[0])
            return false;
 
    return true;
}
 