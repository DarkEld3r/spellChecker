This project is a simple "spell checker".

It has several limitations. At first, only English language is supported. Secondly, only two "edits" can be used to correct the words. Here, an "edit" is either: inserting a single letter or deleting a single letter. With the restriction that if the edits are both insertions or both deletions, they may not be of adjacent characters.

The input will consist of a dictionary followed by a sequence of possibly misspelt words. Both contain words. The dictionary, in free format, is followed by a line containing just the string "===". After this, there will be zero or more text lines containing words followed again by a line containing the string "===". The input is case-insensitive; print corrections from the dictionary in the case they appear in the dictionary and unchanged words from the text lines in their original case as well.



The project structure:
SpellChecker
  shared - common things
  SpellCheckerDll - spell checker implementation (with spellChecker.h public header).
  SpellChecker - console project using ISpellCheckerFacade interface.
  tests - tests for public interface.

Optional Dependencies:
Boost Test Library (for \SpellChecker\tests).



The project does not use exceptions and fancy logging. The reason of this is architectural decision: DLL can be used separately from other project parts. So some specific logging system cannot be used. Cross-dll exception also has some limitations.
