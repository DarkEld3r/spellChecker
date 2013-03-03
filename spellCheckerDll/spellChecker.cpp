#include "stdafx.h"
#define SPELLCHECHER_IMPL
#include "spellChecker.h"
#include "spellCheckerImpl.h"
#include "fileParser.h"

#ifdef _DEBUG
#define VERIFY(x) _ASSERTE(x)
#else
#define VERIFY(x) (x)
#endif // _DEBUG

namespace spellChecker
{
class SpellCheckerFacade : public ISpellCheckerFacade
{
public:
    SpellCheckerFacade()
    {}
    virtual ~SpellCheckerFacade()
    {}

    virtual bool initializeDictionary(const TCHAR* path);
    virtual bool addWord(const std::string& word);

    virtual bool isEmpty() const;

    virtual bool removeWord(const std::string& word);
    virtual void eraseDictionary();

    virtual bool correctWord(const std::string& word, std::string& correctedWord) const;
    virtual std::string correctLine(const std::string& line);
    virtual bool correctFile(const TCHAR* inputFile, const TCHAR* outputFile);

private:
    // If the result parameter is null pointer, then it is initialization of the dictionary.
    // In this case no need to modify the line.
    // If the result parameter is not null pointer, then it is correction, so
    // the result of correction should be stored.
    void parseLine(const std::string& line, std::string* result);
    // Helper function, used in parseLine.
    void processWord(std::string& line, std::string::size_type& pos, 
        std::string::size_type lastPos, bool correction);
    // The function returns false if the word is empty or contains delimiters.
    bool checkWord(const std::string& word) const;

private:
    SimpleSpellChecker m_spellChecker;
    // The sequence of letters (a-z, A-Z) is considered to be a word.
    // All other characters are considered to be delimiters.
    static const char* m_letters;
    static const char* m_endOfDictionary;

private: // Noncopyable.
    SpellCheckerFacade(const SpellCheckerFacade&);
    SpellCheckerFacade& operator=(const SpellCheckerFacade&);
};

const char* SpellCheckerFacade::m_letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char* SpellCheckerFacade::m_endOfDictionary = "===";

bool SpellCheckerFacade::initializeDictionary(const TCHAR* path)
{
    _ASSERTE(path);
    // if length of the path is 1, then it contains only 1 null-terminating character,
    // so the path obviously incorrect.
    if(!path || ::_tcslen(path) <= 1)
    {
        return false;
    }

    FileParser file;
    if(!file.open(path))
    {
        return false;
    }

    std::string line;
    while(file.nextLine(line))
    {
        // The end of the dictionary in the file reached.
        if(m_endOfDictionary == line)
        {
            break;
        }

        // Add words to the dictionary, so the result is null pointer.
        parseLine(line, nullptr /*result*/);
    } // while(file.nextLine(line))

    // If the dictionary is empty - no word has been added.
    return !isEmpty();
}

bool SpellCheckerFacade::addWord(const std::string& word)
{
    return checkWord(word) && m_spellChecker.addWord(word);
}
bool SpellCheckerFacade::isEmpty() const
{
    return m_spellChecker.isDictionaryEmpty();
}

bool SpellCheckerFacade::removeWord(const std::string& word)
{
    return m_spellChecker.removeWord(word);
}

void SpellCheckerFacade::eraseDictionary()
{
    m_spellChecker.eraseDictionary();
}

bool SpellCheckerFacade::correctWord(const std::string& word, std::string& correctedWord) const
{
    // The word must not be empty and should not contain any characters other than letters.
    if(!checkWord(word))
    {
        return false;
    }

    correctedWord = m_spellChecker.correctWord(word);
    return true;
}

std::string SpellCheckerFacade::correctLine(const std::string& line)
{
    std::string result;
    // If the line is empty, then there is nothing to correct.
    // So just return the line.
    if(!line.empty())
    {
        parseLine(line, &result);
    }
    return result;
}

bool SpellCheckerFacade::correctFile(const TCHAR* inputFile, const TCHAR* outputFile)
{
    FileParser file;
    // The file can not be read.
    if(!file.open(inputFile))
    {
        return false;
    }

    // Try to create new file.
    std::fstream output = FileParser::tryOpenCreateFile(outputFile, false /*openExisting*/);
    if(!output.is_open())
    {
        return false;
    }

    // The file should contain the dictionary at the beginning, so skip it.
    if(!file.skipToLine(m_endOfDictionary))
    {
        // Words not found, nothing to do.
        return false;
    }

    std::string line;
    bool firstLine = true;
    while(file.nextLine(line) && m_endOfDictionary != line)
    {
        // The new line is not needed at the end of the file.
        if(!firstLine)
        {
            output << std::endl;
        }
        else
        {
            firstLine = false;
        }
        output << correctLine(line);
    }
    output.close();

    return true;
}

// If the result parameter is null pointer, then it is initialization of the dictionary.
// In this case no need to modify the line.
// If the result parameter is not null pointer, then it is correction, so
// the result of correction should be stored.
void SpellCheckerFacade::parseLine(const std::string& line, std::string* result)
{
    std::string lineCopy(line);

    for(std::string::size_type pos = 0, lastPos = 0;;)
    {
        pos = lineCopy.find_first_not_of(m_letters, lastPos);
    
        // Last word.
        if(pos == std::string::npos)
        {
            pos = lineCopy.length();
            processWord(lineCopy, pos, lastPos, result != nullptr /*correction*/);
            break;
        }
        else
        {
            processWord(lineCopy, pos, lastPos, result != nullptr /*correction*/);
        }
        lastPos = pos + 1;
    } // for(std::string::size_type pos = 0, lastPos = 0;;)

    if(result)
    {
        *result = lineCopy;
    }
}

void SpellCheckerFacade::processWord(std::string& line, std::string::size_type& pos, 
        std::string::size_type lastPos, bool correction)
{
    if(pos == lastPos)
    {
        return;
    }

    const std::string word(line.substr(lastPos, pos - lastPos));
    if(correction)
    {
        std::string correctionResult;
        // The word here can not be empty or invalid, so just use VERIFY.
        VERIFY(correctWord(word, correctionResult));
        // If the length of corrected word is not equal to original word, then
        // need to erase the old word and insert new.
        if(correctionResult.length() != word.length())   
        {
            line.erase(lastPos, pos - lastPos);
            line.insert(lastPos, correctionResult);
            // Move the position because of the changed length.
            pos += correctionResult.length() - word.length();
        }
        else
        {
            // Replace the word with the corrected word.
            line.replace(lastPos, pos - lastPos, correctionResult);
        }
    }
    // The dictionary initialization.
    else
    {
        // The addWord function returns false if the word is already present, 
        // but in this case it is not important.
        addWord(word);
    }
}

bool SpellCheckerFacade::checkWord(const std::string& word) const
{
    return !word.empty() && (word.find_first_not_of(m_letters) == std::string::npos);
}

SPELLCHECKER_API ISpellCheckerFacade* getSpellChecker()
{
    return new SpellCheckerFacade();
}
} // End of spellChecker namespace.
