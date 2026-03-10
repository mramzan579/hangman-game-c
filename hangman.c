/*
 * hangman.c
 * Hangman Game 
 * Set up the project, display the welcome banner,
 * and build the random word selection system.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Constants 
#define WORD_COUNT    10    /* total number of words in the bank */
#define MAX_WRONG      6    /* maximum wrong guesses allowed     */

// Word bank 
/* All the words the game can pick from.
   Player will never see this list during the game. */
const char *word_bank[WORD_COUNT] = {
    "programming",
    "keyboard",
    "monitor",
    "variable",
    "function",
    "compiler",
    "network",
    "database",
    "pointer",
    "algorithm"
};

/* ================================================================
   choose_random_word()
   Picks a random word from the word bank using rand().
   srand() is seeded in main() so every run picks differently.
   Returns a pointer to the chosen word string.
================================================================ */
const char *choose_random_word(void)
{
    int index = rand() % WORD_COUNT;
    return word_bank[index];
}

//Main
int main(void)
{
    const char *word;

    /* Seed the random number generator once at the start */
    srand((unsigned int)time(NULL));

    /* Welcome banner */
    printf("========================================\n");
    printf("            HANGMAN GAME                \n");
    printf("========================================\n");
    printf("  Guess the hidden word one letter at a time.\n");
    printf("  You have %d wrong guesses before game over.\n", MAX_WRONG);
    printf("========================================\n");

    /* Temporary: pick and show a word to confirm it works.
       This debug line will be removed in the next commit. */
    word = choose_random_word();
    printf("\n[DEBUG] Chosen word: %s\n", word);

    return 0;
}