#include "stdafx.h"
#include "../SpellCheckerDll/spellChecker.h"

void showUsage()
{
    tcout << _T("Usage: SpellChecker.exe \"input file\" \"output file\"\n")
        _T("Input file: \n")
        _T("  Path to existent file. Absolute path or the file name for the case,\n")
        _T("  when the file is located in the program directory.\n")
        _T("  This file should contain words for dictionary and words to correct.\n")
        _T("Output file: \n")
        _T("  Path to nonexistent file. Absolute path or the file name for the case,\n")
        _T("  when the file is located in the program directory.\n")
        _T("  If the file exists, than its containt will be erased.\n")
        _T("  This file will contain processed input text.\n")
        _T("Example: \n")
        _T("  SpellChecker.exe input.txt output.txt\n")
        _T("  SpellChecker.exe d:\\input.txt d:\\output.txt")
        << std::endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // Input and output file names are expected in command line.
    if(argc != 3)
    {
        showUsage();
        return -1;
    }

    std::unique_ptr<spellChecker::ISpellCheckerFacade> spellChecker(spellChecker::getSpellChecker());
    if(!spellChecker)
    {
        tcout << _T("getSpellChecker() returns null pointer.") << std::endl;
        return -1;
    }

    if(!spellChecker->initializeDictionary(argv[1]))
    {
        tcout << _T("ISpellCheckerFacade::initializeDictionary() failed.\n") 
            _T("Invalid file format or the file can not be read.") << std::endl;
        showUsage();
        return -1;
    }

    if(!spellChecker->correctFile(argv[1], argv[2]))
    {
        tcout << _T("ISpellCheckerFacade::correctFile() failed.\n")
            _T("Invalid file format or some of the files can not be read.") << std::endl;
        showUsage();
        return -1;
    }

    return 0;
}
