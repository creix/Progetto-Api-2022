# Final Exam Algorithms and Principles of Computer Science 2022

This is the individual final exam for "Algorithms and Principles of Computer Science" course at [Politecnico di Milano](https://www.polimi.it/)

Final Grade: 30/30 Cum Laude

Academic Year: 2021-2022

## Specification

The objective of the project is to create a game similar to wordle, where a user can start a game and try to guess the word from a vocabulary of words.

The user can input all the words contained in the vocabulary before and also during the game, the program keeps track of all the words from the vocabulary compatible with the current constraints learned from the previous attempts.

For each attemp the program have to output if the letter of the word are in the correct position ( **+** ), wrong position ( **|** ) or there are no occurrences of the letter in the word ( **/** ), and also output the total word compatible remaining from the words in the vocabulary.

The program needs also to keep track of all the constrains learned from the various attempt, knowing where a letter can or can't be in the word and the occurrences of every letter in the word.

During a game the user can ask to print all the remaining compatible word in lexicographic order from all the ones present in the complete vocabulary.


### Example

#### Input
```
abcabcabcabcabc    //Word to guess
bbaabccbccbcabc
```

#### Output
```
/+|+++|++/+++++
```

## Implementation
* **Red and Black Tree** for the complete vocabulary
* **Stack** for the remaining compatible word in a game
* **Matrix and Arrays** to save the costraints learned in a game


