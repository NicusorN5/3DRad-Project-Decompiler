#include "File.h"

File::File(char* path)
{
	std::ifstream file(path, std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	Path = new char[strlen(path)+1];
	memset(Path, 0, strlen(path)+1);
	memcpy(Path, path, strlen(path) + 1);

	std::vector<char> buffer(size);
	if (file.read(buffer.data(), size))
	{
		Buffer = (char*)buffer.data();
		Size = buffer.size();
	}
	else
	{
		Buffer = nullptr;
		Size = 0;
	}
}

char File::operator[](int index)
{
	if(Buffer != nullptr) return Buffer[index];
	throw std::exception("Buffer = nullptr");
}

File::~File()
{
	delete[] Path;
	//if(Buffer != nullptr) delete Buffer;
}
