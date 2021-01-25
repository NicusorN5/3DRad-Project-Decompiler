#include "File.h"

File::File(char* path)
{
	std::ifstream file(path, std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<char> buffer(size);
	if (file.read(buffer.data(), size))
	{
		Buffer = (char*)buffer.data();
		Size = buffer.size();
	}
	Buffer = nullptr;
	Size = 0;
}

char File::operator[](int index)
{
	if(Buffer != nullptr) return Buffer[index];
	throw std::exception("Buffer = nullptr");
}

File::~File()
{
	if(Buffer != nullptr)delete Buffer;
}
