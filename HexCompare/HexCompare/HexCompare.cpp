// HexCompare.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include "File.h"
#include <tchar.h>

char* conv(wchar_t* str)
{
    int l = lstrlenW(str);
    char* ret = new char[l + 1];
    int i = 0;
    for (; i < l; i++) ret[i] = (char)str[i];
    ret[i] = 0;
    return ret;
}

//entry point
int main(int argc, char** argv)
{
    //Initialize file instances.

    File *fa = nullptr, *fb = nullptr;

    char* patha = nullptr;
    char* pathb = nullptr;

    std::ofstream result("Result.txt");

    bool ui = false;

    //Work with the command line arguments
    for (int i = 0; i < argc; i++)
    {
        int l = strlen(argv[i]);
        if (strcmp(argv[i], "-ui") == 0) ui = true;
        if (strcmp(argv[i], "-f1") == 0)
        {
            if (argc < (i + 1))
            {
                char errormessage[250];
                sprintf_s(errormessage, "File #1 possibly not specified. \r\n Arguments : i = %d , argv[i] = %s , argc = %d", i, argv[i], argc);
                if (ui)
                {
                    MessageBoxA(nullptr, errormessage, "Command line argument error", MB_OK | MB_ICONERROR);
                }
                else std::cout << "[ERR]" << errormessage << "\r\n";

            }
            patha = argv[i + 1];
        }
        if (strcmp(argv[i], "-f2") == 0)
        {
            if (argc < (i + 1))
            {
                char errormessage[250];
                sprintf_s(errormessage, "File #2 possibly not specified. \r\n Arguments : i = %d , argv[i] = %s , argc = %d", i, argv[i], argc);
                if (ui)
                {
                    MessageBoxA(nullptr, errormessage, "Command line argument error", MB_OK | MB_ICONERROR);
                }
                else std::cout << "[ERR]" << errormessage << "\r\n";
            }
            patha = argv[i + 1];
        }
    }

    //Open files if using Windows UI
    if (ui)
    {
        wchar_t file[MAX_PATH];
        memset(file, 0, sizeof(wchar_t) * MAX_PATH);
        OPENFILENAME opendialog;
        memset(&opendialog, 0, sizeof(OPENFILENAME));
        opendialog.hwndOwner = nullptr;
        opendialog.lpstrInitialDir = L"C:\\";
        opendialog.lpstrTitle = L"Open the first file.";
        opendialog.nMaxFile = MAX_PATH;
        opendialog.lpstrFile = file;
        opendialog.Flags = OFN_FILEMUSTEXIST | OFN_DONTADDTORECENT;
        opendialog.lStructSize = sizeof(opendialog);
        bool a = false;
        while (!a)
        {
            if (GetOpenFileName(&opendialog))
            {
                a = true;
                patha = conv(opendialog.lpstrFile);
                std::cout << "[OK]GUI selected file #1 :" << patha << " \r\n";
            }
        }
        //open dialog again
        a = false; 
        opendialog.lpstrTitle = L"Open the second file";
        while (!a)
        {
            if (GetOpenFileName(&opendialog))
            {
                a = true;
                pathb = conv(opendialog.lpstrFile);
                std::cout << "[OK]GUI selected file #2 :" << pathb << "\r\n";
            }
        }
    }
    //Initialize our vars

    fa = new File(patha);
    fb = new File(pathb);
    int max_size = min(fa->Size, fb->Size);

    //Compare the two buffers
    if (fa->Buffer == nullptr)
    {
        std::cout << "Failed to read the first file.";
        return 1;
    }
    if (fb->Buffer == nullptr)
    {
        std::cout << "Failed to read the second file.";
        return 1;
    }
    std::cout << "[OK] Comparing the two files. \r\n";

    int fcomp = memcmp(fa->Buffer, fb->Buffer, fb->Size);
    if (fcomp != 0)
    {
        std::cout << "The files are different.";
    }
    else
    {
        std::cout << "The files are the same.";
    }

    for (int i = 0; i < max_size; i++)
    {
        bool ca = (*fa)[i], cb = (*fb)[i];
        if (ca != cb)
        {
            std::cout << i << ca << cb << "\r\n";
        }
    }
    delete[] patha;
    delete[] pathb;
    delete fa;
    delete fb;
}
