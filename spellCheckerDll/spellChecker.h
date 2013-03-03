#pragma once
#ifndef SPELLCHECKER_H
#define SPELLCHECKER_H

#include "../shared/tdefs.h"

#ifdef SPELLCHECHER_IMPL
#define SPELLCHECKER_API __declspec(dllexport)
#else
#define SPELLCHECKER_API __declspec(dllimport)
#endif // SPELLCHECHER_IMPL

// Public interface.
namespace spellChecker
{
class ISpellCheckerFacade
{
public:
    virtual ~ISpellCheckerFacade() = 0
    {}

    // This function takes the path to the file with dictionary (and words to check) and
    // initializes the dictionary.
    // This function ignores repeated words (only the first word is added and 
    // influences on the correction order).
    // The function returns false if no word was added or the file can not be read.
    // To correct words from this file call the correctFile function.
    // All words in dictionary are in lower case.
    virtual bool initializeDictionary(const TCHAR* path) = 0;
    // The function adds the word to the dictionary and returns false if the word already presents
    // or if this word is empty or contains invalid characters.
    // The second adding does not influence on the result of the correction.
    virtual bool addWord(const std::string& word) = 0;

    virtual bool isEmpty() const = 0;

    // This function returns false if the word is not present in the dictionary.
    virtual bool removeWord(const std::string& word) = 0;
    // The function erases the dictionary.
    virtual void eraseDictionary() = 0;

    // The function returns false if the word is empty or it contains not only valid characters.
    virtual bool correctWord(const std::string& word, std::string& correctedWord) const = 0;
    // This functions takes the string that contains words. The sequence of letters (a-z, A-Z)
    // is considered to be a word. All other characters are considered to be delimiters.
    // For example the dictionary contains the words "test" and "word". Then the function is called 
    // with the line that contains "tst_wrd" and returns the "test_word" string.
    // The string contains two corrected words and the saved "_" delimiter.
    virtual std::string correctLine(const std::string& line) = 0;
    // The function corrects the file (that also contains the dictionary) and writes the output 
    // to the given file. If the output file exists then its content will be erased.
    // As the input file also contains the dictionary, all its content before the "===" string is
    // ignored, because it is the dictionary.
    // The next "===" string will be considered as the end of the file.
    // The function returns false if the input or output files are invalid or not available.
    virtual bool correctFile(const TCHAR* inputFile, const TCHAR* outputFile) = 0;
};

SPELLCHECKER_API ISpellCheckerFacade* getSpellChecker();

} // End of spellChecker namespace.

#endif // SPELLCHECKER_H
