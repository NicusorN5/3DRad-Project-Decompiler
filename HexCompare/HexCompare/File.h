#pragma once
#include <fstream>
/*
Encapsulates a std::ifstream's char* buffer.
*/
class File
{
public:
	/*
	 Main constructor:
	    Arguments :
		char* path = a C-string containing the path
	*/
	File(char* path);
	/*
	File buffer
	*/
	char* Buffer;
	/*
	Buffer size.
	*/
	size_t Size;

	char operator[](int index);

	~File();
};

