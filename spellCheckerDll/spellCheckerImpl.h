#pragma once
#ifndef SPELLCHECKERIMPL_H
#define SPELLCHECKERIMPL_H

// Private header, should not be used outside spell checker dll.

namespace spellChecker
{
// Simple ANSI case-insensitive spell checker for English language.
class SimpleSpellChecker
{
public:
    SimpleSpellChecker()
    {};

    // The function returns false if the word was not added.
    bool addWord(const std::string& word);

    // The function retuens false if the word is not present in the dictionary.
    bool removeWord(const std::string& word);
    void eraseDictionary();

    bool isDictionaryEmpty() const;

    std::string correctWord(const std::string& word) const;

private:
    // Need to save the position for words in the dictionary to place the 
    // corrected words in the correct order, so the second parameter in the map 
    // indicates the word index in the dictionary.
    typedef std::map<std::string, unsigned int> WordsMapT;
    typedef std::set<std::string> WordsSetT;

    bool isKnownWord(const std::string& word) const;
    WordsMapT getKnownWordsFromSet(const WordsSetT& wordsSet) const;

    // In the case when we call firstEdit from secondEdit, we need "originalword" parameter
    // to skip the words that do not match following requirement:
    // "If the edits are both insertions or both deletions, they may not be of adjacent characters."
    WordsSetT firstEdit(const std::string& word, const std::string& originalWord = "") const;
    // Unlike the firstEdit, secondEdit returns the known words for performance reasons.
    WordsMapT secondEdit(const WordsSetT& firstEditSet, const std::string& originalWord) const;

    std::string stringToLower(const std::string& word) const;

    // Returns the position of the first different character in the strings.
    std::string::size_type findFirstDifCharIndex(const std::string& biggerWord, const std::string& smalerWord) const;

private:
    WordsMapT m_dictionary;
    static const char* m_alphabet;

private: // Noncopyable.
    SimpleSpellChecker(const SimpleSpellChecker&);
    SimpleSpellChecker& operator=(const SimpleSpellChecker&);
};
} // End of spellChecker namespace.

#endif // SPELLCHECKERIMPL_H
