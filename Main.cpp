#include <iostream>
#include <fstream>
#include <filesystem>
#include <Windows.h>

enum class DecompileErrCode
{
	FailedToOpen = 1,
	FailedToSave
};

std::optional<DecompileErrCode> DecompileProject(const std::filesystem::path &compiledProject, const std::filesystem::path& decompiledProjectPath)
{
	std::fstream file_stream(compiledProject,std::ios::in | std::ios::binary | std::ios::ate);
	std::streamsize size = file_stream.tellg();
	file_stream.seekg(0, std::ios::beg);

	std::vector<char> fileBuffer(size);
	if (file_stream.read(fileBuffer.data(), size))
	{
		fileBuffer[0x02] = 0x00;

		fileBuffer[0x08] = 0xB9;
		fileBuffer[0x09] = 0x73;
		fileBuffer[0x0A] = 0xBC;
		fileBuffer[0x0B] = 0xAA;

		file_stream.close();

		file_stream = std::fstream(decompiledProjectPath, std::ios::out | std::ios::binary | std::ios::trunc);

		for (auto byte : fileBuffer)
		{
			file_stream << byte;
		}
		file_stream.flush();
		file_stream.close();
		return {};
	}
	else return DecompileErrCode::FailedToOpen;
}

int WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE unused, _In_ LPSTR cmdArgs, _In_ int unused2)
{
	char filename[_MAX_PATH] = {};

	OPENFILENAMEA ofn{};
	ofn.lStructSize = sizeof(OPENFILENAMEA);
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	ofn.lpstrFile = filename;
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrTitle = "Open a 3D Rad v722 project...";
	
	if (GetOpenFileNameA(&ofn))
	{
		std::filesystem::path compiledProjectPath(filename);
		memset(filename, 0, _MAX_PATH);
		ofn.lpstrTitle = "Save the decompiled 3D Rad v722 project...";
		ofn.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;

		if (GetSaveFileNameA(&ofn))
		{
			auto e = DecompileProject(compiledProjectPath, std::filesystem::path(filename));
			if (e.has_value())
			{
				std::string errMsg;
				switch (e.value())
				{
					case DecompileErrCode::FailedToOpen:
						goto failedToOpen;
					case DecompileErrCode::FailedToSave:
						goto failedToSave;
					default:
						errMsg = std::format("Unknown error. GetLastError() returned {}", GetLastError());
						MessageBoxA(nullptr, errMsg.c_str(), "Error while decompiling", MB_ICONERROR | MB_OK);
						return -3;
				}
			}

			MessageBoxA(nullptr, "Decompilation sucessful!", "", MB_ICONINFORMATION | MB_OK);
			return 0;
		}
		else if (GetLastError() != 0)
		{
failedToSave:
			auto errMsg = std::format("Failed to save the decompiler file! \r\n GetLastError() returned {}", GetLastError());
			MessageBoxA(nullptr, errMsg.c_str(), "Error saving file!", MB_ICONERROR | MB_OK);
			return -2;
		}
	}
	else if (GetLastError() != 0)
	{
failedToOpen:
		auto errMsg = std::format("Failed to open file! \r\n GetLastError() returned {}", GetLastError());
		MessageBoxA(nullptr, errMsg.c_str(), "Error opening file!", MB_ICONERROR | MB_OK);
		return -1;
	}
	else return 0;
}