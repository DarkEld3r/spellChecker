#include "stdafx.h"
#include "spellCheckerImpl.h"

namespace spellChecker
{
const char* SimpleSpellChecker::m_alphabet = "abcdefghijklmnopqrstuvwxyz";

bool SimpleSpellChecker::addWord(const std::string& word)
{
    // It makes no sense to add the empty word in the dictionary.
    if(word.empty())
    {
        return false;
    }

    // The map size is used to create the index of the word.
    // For first word the index is 0, for the second is 1 and so on.
    auto insertResult = m_dictionary.insert(std::make_pair(stringToLower(word), m_dictionary.size()));
    // insertResult.second == false if the word is already present.
    return insertResult.second;
}

bool SimpleSpellChecker::removeWord(const std::string& word)
{
    // map::erase returns the number of the erased words.
    return m_dictionary.erase(word) > 0;
}

void SimpleSpellChecker::eraseDictionary()
{
    m_dictionary.clear();
}

std::string SimpleSpellChecker::correctWord(const std::string& word) const
{
    const std::string lowercasedWord(stringToLower(word));

    // Return the word in lower case to make correction case-insensitive.
    if(isKnownWord(lowercasedWord))
    {
        return lowercasedWord;
    }

    // Save the set of candidates from the first edit, to use it in the second edit,
    // if the first edit contains no known words.
    WordsSetT firstEditSet = firstEdit(lowercasedWord);
    WordsMapT candidatesSet = getKnownWordsFromSet(firstEditSet);
    if(candidatesSet.size() == 0)
    {
        // Nothing is found by the first edit, try the second edit.
        // The secondEdit returns the known words, 
        // there is no need to call "getKnownWordsFromSet" on this set.
        candidatesSet = secondEdit(firstEditSet, lowercasedWord);
    }

    std::string result;
    // Nothing found in the dictionary.
    if(candidatesSet.size() == 0)
    {
        result = "{";
        // Save original case of the word.
        result += word;
        result += "?}";
    }
    // Only one word is found, return this word.
    else if(candidatesSet.size() == 1)
    {
        result = candidatesSet.begin()->first;
    }
    // Several words were found, return the string containing all this words,
    // in this case need to save the words order from dictionary.
    else
    {
        result = "{";
        // Need to transform map<string, unsigned int> to map<unsigned int, string>
        // to get words in correct order.
        std::map<unsigned int, std::string> resultMap;
        std::transform(candidatesSet.begin(), candidatesSet.end(), 
            std::inserter(resultMap, resultMap.end()),
            [](const WordsMapT::value_type& value)
            {
                return std::make_pair(value.second, value.first);
            });
        for(auto iter = resultMap.begin(), end = resultMap.end(); iter != end; ++iter)
        {
            if(result[result.length() - 1] != '{')
            {
                result += " ";
            }
            result += iter->second;
        }
        result += "}";
    }

    return result;
}

bool SimpleSpellChecker::isDictionaryEmpty() const
{
    return m_dictionary.empty();
}

bool SimpleSpellChecker::isKnownWord(const std::string& word) const
{
    return m_dictionary.find(word) != m_dictionary.end();
}

SimpleSpellChecker::WordsMapT SimpleSpellChecker::getKnownWordsFromSet(const WordsSetT& wordsSet) const
{
    WordsMapT knownSet;

    for(auto iter = wordsSet.begin(), end = wordsSet.end(); iter != end; ++iter)
    {
        // Find the word and its position in dictionary.
        auto findResult = m_dictionary.find(*iter);
        if(findResult != m_dictionary.end())
        {
            knownSet.insert(*findResult);
        }
    }

    return knownSet;
}

SimpleSpellChecker::WordsSetT SimpleSpellChecker::firstEdit(const std::string& word, 
    const std::string& originalWord /*= ""*/) const
{
    WordsSetT firstEditSet;

    const std::string::size_type wordLength = word.size();
    // Delete the letters in the word one by one and insert the result word to the set.
    for(std::string::size_type wordPos = 0; wordPos < wordLength; ++wordPos)
    {
        // If the originalWord is not empty - that is the second correction.
        // If the "originalWord"s length is more than the length of the word -
        // one character is already removed. In this case we need to check 
        // whether the deleted characters are not adjacent.
        if(!originalWord.empty() 
            && (originalWord.length() > word.length()))
        {
            // Find the first different character in this strings, to determine
            // which character was previously removed.
            std::string::size_type difPos = findFirstDifCharIndex(originalWord, word);
            // "If the edits are both insertions or both deletions, they may not be of adjacent characters."
            if(difPos == wordPos || (difPos - 1 == wordPos))
            {
                continue;
            }
        }
        firstEditSet.insert(word.substr(0, wordPos) + word.substr(wordPos + 1, wordLength));
    }

    // Insert the letters in the word and insert the result word to the set.
    for(std::string::size_type wordPos = 0; wordPos < wordLength + 1; ++wordPos)
    {
        // If the originalWord is not empty - that is the second correction.
        // If the "originalWord"s length is less than the length of the word -
        // one character is already inserted. In this case we need to check 
        // whether the inserted characters are not adjacent.
        if(!originalWord.empty() 
            && (originalWord.length() < word.length()))
        {
            // Find the first different character in this strings, to determine
            // which character was previously inserted.
            std::string::size_type difPos = findFirstDifCharIndex(word, originalWord);
            // "If the edits are both insertions or both deletions, they may not be of adjacent characters."
            if(difPos == wordPos || difPos + 1 == wordPos)
            {
                continue;
            }
        }
        for(std::size_t alphabetPos = 0, alphabetEnd = strlen(m_alphabet); 
            alphabetPos < alphabetEnd; ++alphabetPos)
        {
            firstEditSet.insert(word.substr(0, wordPos) + m_alphabet[alphabetPos] + 
                word.substr(wordPos, wordLength));
        }
    }

    return firstEditSet;
}

SimpleSpellChecker::WordsMapT SimpleSpellChecker::secondEdit(const WordsSetT& firstEditSet, 
    const std::string& originalWord) const
{
    WordsMapT knownWordsFromSecondEdit;

    for(auto iter = firstEditSet.begin(), end = firstEditSet.end(); iter != end; ++iter)
    {
        // For each word from the set returned by the first edit
        // call again the first edit, to make the second edit.
        const WordsSetT firstEditResult = firstEdit(*iter, originalWord);
        // We have many words here, so insert only the known ones.
        const WordsMapT knownWords = getKnownWordsFromSet(firstEditResult);
        if(knownWords.size() != 0)
        {
            knownWordsFromSecondEdit.insert(knownWords.begin(), knownWords.end());
        }
    }

    return knownWordsFromSecondEdit;
}

std::string SimpleSpellChecker::stringToLower(const std::string& word) const
{
    std::string lowercasedWord;
    std::transform(word.begin(), word.end(), std::inserter(lowercasedWord, lowercasedWord.end()), 
        ::tolower);
    return lowercasedWord;
}

std::string::size_type SimpleSpellChecker::findFirstDifCharIndex(const std::string& biggerWord, 
    const std::string& smalerWord) const
{
    _ASSERTE(biggerWord.length() > smalerWord.length());

    // TODO: std::mismatch?
    std::string::size_type index = 0;
    for(auto iter = smalerWord.begin(), end = smalerWord.end(); iter != end; ++iter)
    {
        if(*iter != biggerWord[index])
        {
            break;
        }
        ++index;
    }

    return index;
}
} // End of spellChecker namespace.
