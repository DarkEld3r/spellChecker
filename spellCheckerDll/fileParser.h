#pragma once
#ifndef FILEPARSER_H
#define FILEPARSER_H

// Private header, should not be used outside the spell checker dll.

namespace spellChecker
{
// Helper class for the file reading and parsing.
class FileParser
{
public:
    FileParser() {}
    ~FileParser();

    bool open(const TCHAR* path);
    void close();
    // Back to the beginning of the file.
    bool reopen();

    // This function not only checks bad bits on fstream, but also check if the file is open.
    bool isGood() const;

    // Call this function repeatedly to walk through the file.
    // The function returns false when reached the end of the file.
    bool nextLine(std::string& line);
    // The file is read until the given string is found.
    // The function returns false if the string is not found.
    // The next nextLine function call returns the next string after "line" string in the file.
    bool skipToLine(const char* line);

    // The Function returns false if rawPath is incorrect or the full path can not be obtained.
    static bool getFullPath(LPCTSTR rawPath, tstring& fullPath);
    // If something is wrong - "empty" (not opened) std::fstream will be returned.
    // For reliability the function makes several attempts to open the file and
    // waits some time between attempts.
    static std::fstream tryOpenCreateFile(LPCTSTR path, bool openExisting, unsigned int attemptsCount = 5);

private:
    std::fstream m_file;

private: // Noncopyable.
    FileParser(const FileParser&);
    FileParser& operator=(const FileParser&);
};
} // End of spellChecker namespace.

#endif // FILEPARSER_H
