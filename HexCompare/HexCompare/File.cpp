#include "File.h"

File::File(char* path)
{
	size_t size = 0;
	char temp;
	std::ifstream in(path,std::ios::in || std::ios::binary);
	while (in >> temp) size++;
	Size = size;
	Buffer = new char[size];
	in.seekg(0);
	for (uint32_t i = 0; i < size; i++) in >> Buffer[i];
	in.close();
}

char File::operator[](int index)
{
	return Buffer[index];
}

File::~File()
{
	if(Buffer != nullptr)delete Buffer;
}
