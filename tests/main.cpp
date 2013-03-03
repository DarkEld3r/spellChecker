#include "stdafx.h"
#include "../SpellCheckerDll/spellChecker.h"

const TCHAR* g_emptyFile = _T("test_empty.txt");
const TCHAR* g_invalidFile = _T("test_invalid.txt");
const TCHAR* g_nonexistentextFile = _T("nonexistentFileF2B5DCB8-BE9C-4826-A294-A64FB26E2353.txt");

const TCHAR* g_simpleFile = _T("test_simple.txt");
const TCHAR* g_simpleOutputFile = _T("test_simple_output.txt");
const TCHAR* g_simpleOutputEmptyDictionaryFile = _T("test_simple_output_empty_dictionary.txt");

const TCHAR* g_complexFile = _T("test_complex.txt");
const TCHAR* g_complexOutputFile = _T("test_complex_output.txt");
const TCHAR* g_complexOutputEmptyDictionaryFile = _T("test_complex_output_empty_dictionary.txt");

const TCHAR* g_outputFile = _T("test_output.txt");

// Safe in release, may throw in debug.
inline boost::format safeFormat(const char *formatString)
{
    boost::format fmt;

#ifdef _DEBUG 
    fmt.exceptions(boost::io::all_error_bits);
#else // not _DEBUG.
    fmt.exceptions(boost::io::no_error_bits); // Set no exceptions for boost::format in release.
#endif // _DEBUG.

    fmt.parse(formatString);
    return fmt;
}

// Creates formated string.
#define FORMAT_STR(string, arguments) \
    (safeFormat(string) arguments).str().c_str()

BOOST_AUTO_TEST_CASE(initializeDictionary)
{
    std::unique_ptr<spellChecker::ISpellCheckerFacade> spellChecker(spellChecker::getSpellChecker());
    BOOST_REQUIRE_MESSAGE(spellChecker, "getSpellChecker() function should not return a null pointer.");
    BOOST_REQUIRE_MESSAGE(spellChecker->isEmpty(), "Dictionary should be empty.");
    
    // Call with invalid argumets.
#ifndef _DEBUG
    // Suppress assertions.
    BOOST_REQUIRE_MESSAGE(!spellChecker->initializeDictionary(NULL), 
        "ISpellCheckerFacade::initializeDictionary(NULL) should return false.");
    BOOST_REQUIRE_MESSAGE(spellChecker->isEmpty(), "Dictionary should be empty.");
#endif // _DEBUG

    // Empty path.
    BOOST_REQUIRE_MESSAGE(!spellChecker->initializeDictionary(_T("")), 
        "ISpellCheckerFacade::initializeDictionary(\"\") should return false.");
    BOOST_REQUIRE_MESSAGE(spellChecker->isEmpty(), "Dictionary should be empty.");

    // Empty file.
    BOOST_REQUIRE_MESSAGE(!spellChecker->initializeDictionary(g_emptyFile), 
        FORMAT_STR("ISpellCheckerFacade::initializeDictionary(%1%) should return false.",
        % CT2CA(g_emptyFile)));
    BOOST_REQUIRE_MESSAGE(spellChecker->isEmpty(), "Dictionary should be empty.");

    // Invalid file (the contains no the words).
    BOOST_REQUIRE_MESSAGE(!spellChecker->initializeDictionary(g_invalidFile), 
        FORMAT_STR("ISpellCheckerFacade::initializeDictionary(%1%) should return false.",
        % CT2CA(g_invalidFile)));
    BOOST_REQUIRE_MESSAGE(spellChecker->isEmpty(), "Dictionary should be empty.");

    // Nonexistent file.
    BOOST_REQUIRE_MESSAGE(!spellChecker->initializeDictionary(g_nonexistentextFile), 
        FORMAT_STR("ISpellCheckerFacade::initializeDictionary(%1%) should return false.",
        % CT2CA(g_nonexistentextFile)));
    BOOST_REQUIRE_MESSAGE(spellChecker->isEmpty(), "Dictionary should be empty.");

    // Valid call.
    BOOST_REQUIRE_MESSAGE(spellChecker->initializeDictionary(g_simpleFile), 
        FORMAT_STR("ISpellCheckerFacade::initializeDictionary(%1%) should return true.",
        % CT2CA(g_simpleFile)));
    BOOST_REQUIRE_MESSAGE(!spellChecker->isEmpty(), "Dictionary should NOT be empty.");

    // Try second call.
    BOOST_REQUIRE_MESSAGE(spellChecker->initializeDictionary(g_simpleFile), 
        FORMAT_STR("Secont call to ISpellCheckerFacade::initializeDictionary(%1%) should return true.",
        % CT2CA(g_simpleFile)));
    BOOST_REQUIRE_MESSAGE(!spellChecker->isEmpty(), "Dictionary should NOT be empty.");

    // Erase dictionary and try once more.
    spellChecker->eraseDictionary();
    BOOST_REQUIRE_MESSAGE(spellChecker->isEmpty(),
        "Dictionary should be empty after ISpellCheckerFacade::eraseDictionary call.");
    BOOST_REQUIRE_MESSAGE(spellChecker->initializeDictionary(g_simpleFile), 
        FORMAT_STR("ISpellCheckerFacade::initializeDictionary(%1%) should return true.",
        % CT2CA(g_simpleFile)));
    BOOST_REQUIRE_MESSAGE(!spellChecker->isEmpty(), "Dictionary should NOT be empty.");
}

BOOST_AUTO_TEST_CASE(addWordToDictionary)
{
    std::unique_ptr<spellChecker::ISpellCheckerFacade> spellChecker(spellChecker::getSpellChecker());
    BOOST_REQUIRE_MESSAGE(spellChecker, "getSpellChecker() function should not return a null pointer.");
    BOOST_REQUIRE_MESSAGE(spellChecker->isEmpty(), "Dictionary should be empty");

    // Invalid arguments - empty word.
    BOOST_REQUIRE_MESSAGE(!spellChecker->addWord(""), "Empty word.");
    BOOST_REQUIRE_MESSAGE(spellChecker->isEmpty(), "Dictionary should be empty.");

    // Invalid arguments - incorrect word.
    BOOST_REQUIRE_MESSAGE(!spellChecker->addWord("1234567890-=`~!@# $%^&*()_+[{"), "Incorrect word.");
    BOOST_REQUIRE_MESSAGE(spellChecker->isEmpty(), "Dictionary should be empty.");

    // Valid call.
    BOOST_REQUIRE_MESSAGE(spellChecker->addWord("test"), "Valid call (word 'test').");
    BOOST_REQUIRE_MESSAGE(!spellChecker->isEmpty(), "Dictionary should NOT be empty");

    BOOST_REQUIRE_MESSAGE(spellChecker->addWord("word"), "Valid call (word 'word').");
    BOOST_REQUIRE_MESSAGE(!spellChecker->isEmpty(), "Dictionary should NOT be empty");

    // Invalid call - the word has already been added.
    BOOST_REQUIRE_MESSAGE(!spellChecker->addWord("test"), "The word has already been added.");
    BOOST_REQUIRE_MESSAGE(!spellChecker->addWord("word"), "The word has already been added.");

    // Try to remove the words and add them again.
    spellChecker->eraseDictionary();
    BOOST_REQUIRE_MESSAGE(spellChecker->addWord("test"), "Valid call (word 'test').");
    BOOST_REQUIRE_MESSAGE(!spellChecker->isEmpty(), "Dictionary should NOT be empty");

    BOOST_REQUIRE_MESSAGE(spellChecker->addWord("word"), "Valid call (word 'word').");
    BOOST_REQUIRE_MESSAGE(!spellChecker->isEmpty(), "Dictionary should NOT be empty");
}

BOOST_AUTO_TEST_CASE(isEmpty)
{
    std::unique_ptr<spellChecker::ISpellCheckerFacade> spellChecker(spellChecker::getSpellChecker());
    BOOST_REQUIRE_MESSAGE(spellChecker, "getSpellChecker() function should not return a null pointer.");
    BOOST_REQUIRE_MESSAGE(spellChecker->isEmpty(), "Dictionary should be empty");

    // Try to initialize the dictionary with the invalid file.
    // The dictionary should remain empty.
    BOOST_REQUIRE_MESSAGE(!spellChecker->initializeDictionary(g_invalidFile), 
        FORMAT_STR("ISpellCheckerFacade::initializeDictionary(%1%) should return false.",
        % CT2CA(g_invalidFile)));
    BOOST_REQUIRE_MESSAGE(spellChecker->isEmpty(), "Dictionary should be empty.");

    // Valid file - the dictionary should not be empty.
    BOOST_REQUIRE_MESSAGE(spellChecker->initializeDictionary(g_simpleFile), 
        FORMAT_STR("ISpellCheckerFacade::initializeDictionary(%1%) should return true.",
        % CT2CA(g_simpleFile)));
    BOOST_REQUIRE_MESSAGE(!spellChecker->isEmpty(), "Dictionary should NOT be empty.");

    // Erase the dictionary - should be empty again.
    spellChecker->eraseDictionary();
    BOOST_REQUIRE_MESSAGE(spellChecker->isEmpty(), "Dictionary should be empty.");

    // Add word with ISpellCheckerFacade::addWord.
    BOOST_REQUIRE_MESSAGE(spellChecker->addWord("test"), "Add word('test')");
    BOOST_REQUIRE_MESSAGE(!spellChecker->isEmpty(), "Dictionary should NOT be empty");

    BOOST_REQUIRE_MESSAGE(spellChecker->addWord("word"), "Add word('word').");
    BOOST_REQUIRE_MESSAGE(!spellChecker->isEmpty(), "Dictionary should NOT be empty");

    // Remove words.
    BOOST_REQUIRE_MESSAGE(spellChecker->removeWord("test"), "removeWord('test').");
    BOOST_REQUIRE_MESSAGE(!spellChecker->isEmpty(), "Dictionary should NOT be empty");

    BOOST_REQUIRE_MESSAGE(spellChecker->removeWord("word"), "removeWord('word').");
    BOOST_REQUIRE_MESSAGE(spellChecker->isEmpty(), "Dictionary should be empty");
}

BOOST_AUTO_TEST_CASE(removeWordFromDictionary)
{
    std::unique_ptr<spellChecker::ISpellCheckerFacade> spellChecker(spellChecker::getSpellChecker());
    BOOST_REQUIRE_MESSAGE(spellChecker, "getSpellChecker() function should not return a null pointer.");
    BOOST_REQUIRE_MESSAGE(spellChecker->isEmpty(), "Dictionary should be empty");

    BOOST_REQUIRE_MESSAGE(spellChecker->addWord("test"), "Add word('test')");
    BOOST_REQUIRE_MESSAGE(!spellChecker->isEmpty(), "Dictionary should NOT be empty");

    BOOST_REQUIRE_MESSAGE(spellChecker->removeWord("test"), "removeWord('test').");
    BOOST_REQUIRE_MESSAGE(!spellChecker->removeWord("test"), 
        "removeWord('test') should return false for the already removed word.");

    BOOST_REQUIRE_MESSAGE(spellChecker->addWord("word"), "Add word('word').");
    BOOST_REQUIRE_MESSAGE(!spellChecker->isEmpty(), "Dictionary should NOT be empty");

    BOOST_REQUIRE_MESSAGE(spellChecker->removeWord("word"), "removeWord('word').");
    BOOST_REQUIRE_MESSAGE(!spellChecker->removeWord("word"), 
        "removeWord('word') should return false for the already removed word.");
}

BOOST_AUTO_TEST_CASE(eraseDictionary)
{
    std::unique_ptr<spellChecker::ISpellCheckerFacade> spellChecker(spellChecker::getSpellChecker());
    BOOST_REQUIRE_MESSAGE(spellChecker, "getSpellChecker() function should not return a null pointer.");
    BOOST_REQUIRE_MESSAGE(spellChecker->isEmpty(), "Dictionary should be empty");

    // Valid call for the empty dictionary.
    spellChecker->eraseDictionary();
    BOOST_REQUIRE_MESSAGE(spellChecker->isEmpty(), "Dictionary should be empty");

    // Valid file - the dictionary should not be empty.
    BOOST_REQUIRE_MESSAGE(spellChecker->initializeDictionary(g_simpleFile), 
        FORMAT_STR("ISpellCheckerFacade::initializeDictionary(%1%) should return true.",
        % CT2CA(g_simpleFile)));
    BOOST_REQUIRE_MESSAGE(!spellChecker->isEmpty(), "Dictionary should NOT be empty.");

    // Valid call for the not empty dictionary.
    spellChecker->eraseDictionary();
    BOOST_REQUIRE_MESSAGE(spellChecker->isEmpty(), "Dictionary should be empty");

    BOOST_REQUIRE_MESSAGE(spellChecker->addWord("word"), "Add word('word').");
    BOOST_REQUIRE_MESSAGE(!spellChecker->isEmpty(), "Dictionary should NOT be empty");

    spellChecker->eraseDictionary();
    BOOST_REQUIRE_MESSAGE(spellChecker->isEmpty(), "Dictionary should be empty");
}

BOOST_AUTO_TEST_CASE(correctWord)
{
    std::unique_ptr<spellChecker::ISpellCheckerFacade> spellChecker(spellChecker::getSpellChecker());
    BOOST_REQUIRE_MESSAGE(spellChecker, "getSpellChecker() function should not return a null pointer.");
    BOOST_REQUIRE_MESSAGE(spellChecker->isEmpty(), "Dictionary should be empty");

    std::string result;
    // Invalid call - the empty word.
    BOOST_REQUIRE_MESSAGE(!spellChecker->correctWord("", result), 
        "ISpellCheckerFacade::correctWord() should return false for the empty word.");

    // Invalid call - the word contains invalid characters.
    BOOST_REQUIRE_MESSAGE(!spellChecker->correctWord("1234567890-=`~!@# $%^&*()_+[", result), 
        "ISpellCheckerFacade::correctWord() should return false for the invalid word.");
    BOOST_REQUIRE_MESSAGE(!spellChecker->correctWord("     ", result), 
        "ISpellCheckerFacade::correctWord() should return false for the invalid word.");
    BOOST_REQUIRE_MESSAGE(!spellChecker->correctWord("test!", result), 
        "ISpellCheckerFacade::correctWord() should return false for the invalid word.");
    BOOST_REQUIRE_MESSAGE(!spellChecker->correctWord("!test", result), 
        "ISpellCheckerFacade::correctWord() should return false for the invalid word.");

    const struct
    {
        const char* wordToAdd;
        const char* wordToCorrect;
        const char* invalidResult;
        const char* correctResult;
    } testWords[] = 
    {
        // The input is case-insensitive, but if the word is unknown, 
        // then the original input should be saved.
        { "TEST", "tEs",       "{tEs?}",       "test"       },
        { "TeSt", "TES",       "{TES?}",       "test"       },
        { "word", "TEST",      "{TEST?}",      "{TEST?}"    },
        // Insertion.
        { "test", "est",       "{est?}",       "test"         },
        { "test", "tst",       "{tst?}",       "test"         },
        { "test", "tet",       "{tet?}",       "test"         },
        { "test", "tes",       "{tes?}",       "test"         },
        // Both edits are insertion.
        { "test", "ts",        "{ts?}",        "test"         },
        { "test", "et",        "{et?}",        "test"         },
        // Both edits are adjacent insertion.
        { "test", "te",        "{te?}",        "{te?}"        },
        { "test", "st",        "{st?}",        "{st?}"        },
        { "test", "tt",        "{tt?}",        "{tt?}"        },
        // More insertions required.
        { "test", "t",         "{t?}",         "{t?}"         },
        { "test", "e",         "{e?}",         "{e?}"         },
        { "test", "s",         "{s?}",         "{s?}"         },
        // Deletion.
        { "test", "atest",     "{atest?}",     "test"         },
        { "test", "tbest",     "{tbest?}",     "test"         },
        { "test", "tecst",     "{tecst?}",     "test"         },
        { "test", "tesdt",     "{tesdt?}",     "test"         },
        { "test", "teste",     "{teste?}",     "test"         },
        // Both edits are deletion.
        { "test", "atestb",    "{atestb?}",    "test"         },
        { "test", "tcedst",    "{tcedst?}",    "test"         },
        { "test", "tefsgt",    "{tefsgt?}",    "test"         },
        { "test", "tyestz",    "{tyestz?}",    "test"         },
        // Both edits are adjacent deletion.
        { "test", "abtest",    "{abtest?}",    "{abtest?}"    },
        { "test", "tcdest",    "{tcdest?}",    "{tcdest?}"    },
        { "test", "testgy",    "{testgy?}",    "{testgy?}"    },
        // More deletions required.
        { "test", "taebsct",   "{taebsct?}",   "{taebsct?}"   },
        { "test", "ctdefsxto", "{ctdefsxto?}", "{ctdefsxto?}" },
        { "test", "tezsptl",   "{tezsptl?}",   "{tezsptl?}"   },
    };

    for(int i = 0; i < _countof(testWords); ++i)
    {
        // At first, try to correct the word without adding it to the dictionary.
        BOOST_REQUIRE_MESSAGE(spellChecker->correctWord(testWords[i].wordToCorrect, result), 
            FORMAT_STR("ISpellCheckerFacade::correctWord('%1%')",
            % testWords[i].wordToCorrect));

        // The result should be equal to the testWords[i].invalidResult.
        BOOST_CHECK_MESSAGE(testWords[i].invalidResult == result,
            FORMAT_STR("'%1%' == '%2%'",
            % testWords[i].invalidResult % result.c_str()));

        BOOST_REQUIRE_MESSAGE(spellChecker->addWord(testWords[i].wordToAdd), 
            FORMAT_STR("ISpellCheckerFacade::addWord('%1%')",
            % testWords[i].wordToAdd));

        BOOST_REQUIRE_MESSAGE(spellChecker->correctWord(testWords[i].wordToCorrect, result), 
            FORMAT_STR("ISpellCheckerFacade::correctWord('%1%')",
            % testWords[i].wordToCorrect));

        // The result should be equal to the testWords[i].correctResult.
        BOOST_CHECK_MESSAGE(testWords[i].correctResult == result,
            FORMAT_STR("'%1%' == '%2%'",
            % testWords[i].invalidResult % result.c_str()));

        spellChecker->eraseDictionary();
    }
}

BOOST_AUTO_TEST_CASE(correctLine)
{
    std::unique_ptr<spellChecker::ISpellCheckerFacade> spellChecker(spellChecker::getSpellChecker());
    BOOST_REQUIRE_MESSAGE(spellChecker, "getSpellChecker() function should not return a null pointer.");
    BOOST_REQUIRE_MESSAGE(spellChecker->isEmpty(), "Dictionary should be empty");

    const struct
    {
        const char* line;
        const char* expectedResult;
    } testEmptyDictionary[] = 
    {
        { "", "" },
        { " ", " " },
        { "     ", "     " },
        { "1234567890", "1234567890" },
        { "word", "{word?}" },
        { " this_is123a^^^test ", " {this?}_{is?}123{a?}^^^{test?} " },
        { "1234567890-=`~!@# $^&*()_+[{]};:'\"\\|,<.>/?", "1234567890-=`~!@# $^&*()_+[{]};:'\"\\|,<.>/?" },
        { "one more test", "{one?} {more?} {test?}" },
    };

    std::string result;
    // At first, try some string with empty dictionary.
    for(int i = 0; i < _countof(testEmptyDictionary); ++i)
    {
        result = spellChecker->correctLine(testEmptyDictionary[i].line);
        BOOST_CHECK_MESSAGE(testEmptyDictionary[i].expectedResult == result,
            FORMAT_STR("'%1%' == '%2%'",
            % testEmptyDictionary[i].expectedResult % result.c_str()));
    }

    // Add some words.
    BOOST_REQUIRE_MESSAGE(spellChecker->addWord("this"), "ISpellCheckerFacade::addWord(this)");
    BOOST_REQUIRE_MESSAGE(spellChecker->addWord("is"), "ISpellCheckerFacade::addWord(this)");
    BOOST_REQUIRE_MESSAGE(spellChecker->addWord("a"), "ISpellCheckerFacade::addWord(this)");
    BOOST_REQUIRE_MESSAGE(spellChecker->addWord("test"), "ISpellCheckerFacade::addWord(this)");

    const struct
    {
        const char* line;
        const char* expectedResult;
    } testNotEmptyDictionary[] = 
    {
        { "", "" },
        { " ", " " },
        { "     ", "     " },
        { "1234567890", "1234567890" },
        { "word", "{word?}" },
        { " this_is123a^^^test ", " this_is123a^^^test " },
        { "1234567890-=`~!@# $^&*()_+[{]};:'\"\\|,<.>/?", "1234567890-=`~!@# $^&*()_+[{]};:'\"\\|,<.>/?" },
        { "one more test", "{one?} {more?} test" },
        { "this", "this" },
        { "is", "is" },
        { "a", "a" },
        { "test", "test" },
    };

    for(int i = 0; i < _countof(testEmptyDictionary); ++i)
    {
        result = spellChecker->correctLine(testNotEmptyDictionary[i].line);
        BOOST_CHECK_MESSAGE(testNotEmptyDictionary[i].expectedResult == result,
            FORMAT_STR("'%1%' == '%2%'",
            % testNotEmptyDictionary[i].expectedResult % result.c_str()));
    }
}

// This is a test so use a simplified version.
tstring getFullPath(const TCHAR* path)
{
    _ASSERTE(path);

    std::unique_ptr<TCHAR[]> pathToExe(new TCHAR[_MAX_PATH + 1]);
    DWORD result = ::GetModuleFileName(NULL, pathToExe.get(), _MAX_PATH);
    BOOST_REQUIRE(result != 0);
    ::PathRemoveFileSpec(pathToExe.get());
    tstring fullPath = pathToExe.get();
    fullPath += _T("\\");
    fullPath += path;

    return fullPath;
}

void compareFiles(const TCHAR* first, const TCHAR* second)
{
    _ASSERTE(first);
    _ASSERTE(second);

    // Make full path.
    tstring firstFullPath(getFullPath(first));
    std::ifstream firstFile(firstFullPath);
    BOOST_REQUIRE_MESSAGE(firstFile.is_open() && firstFile.good(),
        FORMAT_STR("%1% file should be valid.",
        % CT2CA(first)));

    tstring secondFullPath(getFullPath(second));
    std::ifstream secondFile(secondFullPath);
    BOOST_REQUIRE_MESSAGE(secondFile.is_open() && secondFile.good(),
        FORMAT_STR("%1% file should be valid.",
        % CT2CA(second)));

    std::string firstLine, secondLine;
    while(firstFile.good() && secondFile.good())
    {
        std::getline(firstFile, firstLine);
        std::getline(secondFile, secondLine);
        BOOST_REQUIRE_MESSAGE(firstLine == secondLine,
            FORMAT_STR("%1% == %2%",
            % firstLine.c_str() % secondLine.c_str()));
    }

    // Both file should have the same length.
    BOOST_REQUIRE_MESSAGE(!firstFile.good() && !secondFile.good(),
        FORMAT_STR("%1% and %2% file should have the same lengh.",
        % first % second));
}

BOOST_AUTO_TEST_CASE(correctFile)
{
    std::unique_ptr<spellChecker::ISpellCheckerFacade> spellChecker(spellChecker::getSpellChecker());
    BOOST_REQUIRE_MESSAGE(spellChecker, "getSpellChecker() function should not return a null pointer.");
    BOOST_REQUIRE_MESSAGE(spellChecker->isEmpty(), "Dictionary should be empty");

    // Delete the output file at the end of the tests.
    // BOOL DeleteFile(LPCTSTR lpFileName)
    std::unique_ptr<const TCHAR, std::function<BOOL(LPCTSTR)>> deleteFile(g_outputFile, ::DeleteFile);

    // Invalid call - the input file is empty.
    BOOST_REQUIRE_MESSAGE(!spellChecker->correctFile(g_emptyFile, g_outputFile),
        FORMAT_STR("ISpellCheckerFacade::initializeDictionary(%1%, %2%) should return false.",
        % CT2CA(g_emptyFile) % CT2CA(g_outputFile)));

    // Invalid call - the input file is invalid.
    BOOST_REQUIRE_MESSAGE(!spellChecker->correctFile(g_invalidFile, g_outputFile),
        FORMAT_STR("ISpellCheckerFacade::initializeDictionary(%1%, %2%) should return false.",
        % CT2CA(g_invalidFile) % CT2CA(g_outputFile)));

    // Invalid call - the output file path is empty.
    BOOST_REQUIRE_MESSAGE(!spellChecker->correctFile(g_simpleFile, _T("")),
        FORMAT_STR("ISpellCheckerFacade::initializeDictionary(%1%, '') should return false.",
        % CT2CA(g_simpleFile)));

    // Valid call, but the dictionary is empty.
    BOOST_REQUIRE_MESSAGE(spellChecker->correctFile(g_simpleFile, g_outputFile),
        FORMAT_STR("ISpellCheckerFacade::initializeDictionary(%1%, %2%) should return false.",
        % CT2CA(g_simpleFile) % CT2CA(g_outputFile)));

    compareFiles(g_outputFile, g_simpleOutputEmptyDictionaryFile);

    BOOST_REQUIRE_MESSAGE(spellChecker->initializeDictionary(g_simpleFile), 
        FORMAT_STR("ISpellCheckerFacade::initializeDictionary(%1%) should return true.",
        % CT2CA(g_simpleFile)));
    BOOST_REQUIRE_MESSAGE(!spellChecker->isEmpty(), "Dictionary should NOT be empty.");

    BOOST_REQUIRE_MESSAGE(spellChecker->correctFile(g_simpleFile, g_outputFile),
        FORMAT_STR("ISpellCheckerFacade::initializeDictionary(%1%, %2%) should return false.",
        % CT2CA(g_simpleFile) % CT2CA(g_outputFile)));

    compareFiles(g_outputFile, g_simpleOutputFile);

    spellChecker->eraseDictionary();
    BOOST_REQUIRE_MESSAGE(spellChecker->isEmpty(), "Dictionary should be empty.");

    // Valid call, but the dictionary is empty.
    BOOST_REQUIRE_MESSAGE(spellChecker->correctFile(g_complexFile, g_outputFile),
        FORMAT_STR("ISpellCheckerFacade::initializeDictionary(%1%, %2%) should return false.",
        % CT2CA(g_complexFile) % CT2CA(g_outputFile)));

    compareFiles(g_outputFile, g_complexOutputEmptyDictionaryFile);

    BOOST_REQUIRE_MESSAGE(spellChecker->initializeDictionary(g_complexFile), 
        FORMAT_STR("ISpellCheckerFacade::initializeDictionary(%1%) should return true.",
        % CT2CA(g_complexFile)));
    BOOST_REQUIRE_MESSAGE(!spellChecker->isEmpty(), "Dictionary should NOT be empty.");

    BOOST_REQUIRE_MESSAGE(spellChecker->correctFile(g_complexFile, g_outputFile),
        FORMAT_STR("ISpellCheckerFacade::initializeDictionary(%1%, %2%) should return false.",
        % CT2CA(g_emptyFile) % CT2CA(g_outputFile)));

    compareFiles(g_outputFile, g_complexOutputFile);
}
