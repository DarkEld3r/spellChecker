#include "stdafx.h"
#include "fileParser.h"

namespace spellChecker
{
FileParser::~FileParser()
{
    close();
}

bool FileParser::open(const TCHAR* path)
{
    _ASSERTE(path);
    if(!path || ::_tcslen(path) <= 1)
    {
        return false;
    }

    close();
    m_file = tryOpenCreateFile(path, true /*openExisting*/);
    return isGood();
}

void FileParser::close()
{
    if(m_file.is_open())
    {
        m_file.close();
    }
    // Clear the file state.
    m_file.clear();
}

bool FileParser::reopen()
{
    if(!m_file.is_open())
    {
        return false;
    }

    // Clear the file state.
    m_file.clear();
    // Go to beginning.
    m_file.seekg(0, std::ios::beg);
    return isGood();
}

bool FileParser::isGood() const
{
    return m_file.good() && m_file.is_open();
}

bool FileParser::nextLine(std::string& line)
{
    if(!isGood())
    {
        return false;
    }

    getline(m_file, line);
    return true;
}

bool FileParser::skipToLine(const char* line)
{
    _ASSERTE(line);
    if(!line || ::strlen(line) <= 1)
    {
        return false;
    }

    // Read strings from file until the string match with string from line parameter.
    std::string readedLine;
    while(nextLine(readedLine))
    {
        if(readedLine == line)
        {
            return true;
        }
    }
    return false;
}

bool FileParser::getFullPath(LPCTSTR rawPath, tstring& fullPath)
{
    _ASSERTE(rawPath);
    if(!rawPath || ::_tcslen(rawPath) <= 1)
    {
        fullPath = _T("");
        return false;
    }

    fullPath = rawPath;

    if(::PathIsRelative(fullPath.c_str()))
    {
        DWORD size = _MAX_PATH;
        std::unique_ptr<TCHAR[]> pathToExe(new TCHAR[size + 1]);
        for(;;)
        {
            size = ::GetModuleFileName(NULL, pathToExe.get(), size);
            // If the GetModuleFileName function is failed - 
            // we can't do anything.
            if(size == 0)
            {
                fullPath = _T("");
                return false;
            }
            else if(::GetLastError() == ERROR_INSUFFICIENT_BUFFER)
            {
                size *= 2;
                pathToExe.reset(new TCHAR[size + 1]);
            }
            else
            {
                break;
            }
        } // for(;;)

        ::PathRemoveFileSpec(pathToExe.get());
        fullPath = pathToExe.get();
        fullPath += _T("\\");
        fullPath += rawPath;
    }

    return true;
}

std::fstream FileParser::tryOpenCreateFile(LPCTSTR path, bool openExisting, 
    unsigned int attemptsCount /*= 5*/)
{
    _ASSERTE(path);

    tstring fullPath;
    getFullPath(path, fullPath);
    std::fstream fileStream;
    // If the getFullPath function failed - the fullPath is empty,
    // in this case, it makes no sense to make attempts to open the file.
    if(fullPath.empty())
    {
        return fileStream;
    }

    do
    {
        if(openExisting)
        {
            fileStream.open(fullPath, std::ios_base::in);
        }
        else
        {
            fileStream.open(fullPath, std::ios_base::out | std::ios_base::trunc);
        }
    // Make several attempts to open file for reliability.
    } while(--attemptsCount > 0 
        && !fileStream.is_open() 
        && []() -> bool { ::Sleep(100); return true; }() );

    return fileStream;
}
} // End of spellChecker namespace.
