/*
 * hangman.c
 * Hangman Game — C Version
 * Add the progress array, letter guessing, and word reveal logic.
 * The player can now guess letters and see them revealed.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>   /* strlen() — measures word length */
#include <time.h>

/* ── Constants ──────────────────────────────────────────────── */
#define WORD_COUNT    10
#define MAX_WRONG      6
#define MAX_WORD_LEN  20   /* maximum length of any word in the bank */

/* ── Word bank ──────────────────────────────────────────────── */
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
   clear_input_buffer()
   Drains leftover characters from stdin after every scanf.
   Without this, bad input causes the game to loop forever.
================================================================ */
void clear_input_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

/* ================================================================
   choose_random_word()
   Picks a random word from the word bank using rand().
   Returns a pointer to the chosen word string.
================================================================ */
const char *choose_random_word(void)
{
    int index = rand() % WORD_COUNT;
    return word_bank[index];
}

/* ================================================================
   display_word_progress()
   Prints the current state of the word.
   Correctly guessed letters show as the letter.
   Unguessed letters show as underscores.
   Example: p r o _ r a _ _ i n _
   Parameters:
     word     — the secret word
     progress — array same length as word, holds guessed letters
                or '_' for positions not yet guessed
================================================================ */
void display_word_progress(const char *word, char *progress)
{
    int i;
    int len = strlen(word);

    printf("  Word: ");
    for (i = 0; i < len; i++) {
        printf("%c ", progress[i]);
    }
    printf("\n");
}

/* ================================================================
   update_progress()
   Checks if the guessed letter exists anywhere in the word.
   If found — reveals it in the progress array at every match.
   Returns 1 if the letter was found, 0 if not found.
   Parameters:
     word     — the secret word
     progress — the current progress array to update
     letter   — the letter the player just guessed
================================================================ */
int update_progress(const char *word, char *progress, char letter)
{
    int i;
    int found = 0;
    int len   = strlen(word);

    for (i = 0; i < len; i++) {
        if (word[i] == letter) {
            progress[i] = letter;   /* reveal this position */
            found = 1;
        }
    }
    return found;
}

/* ── Main ───────────────────────────────────────────────────── */
int main(void)
{
    const char *word;
    char        progress[MAX_WORD_LEN];
    int         i, len;
    char        letter;

    /* Seed random number generator */
    srand((unsigned int)time(NULL));

    /* Welcome banner */
    printf("========================================\n");
    printf("            HANGMAN GAME                \n");
    printf("========================================\n");
    printf("  Guess the hidden word one letter at a time.\n");
    printf("  You have %d wrong guesses before game over.\n", MAX_WRONG);
    printf("========================================\n");

    /* Pick a word and set up the progress array */
    word = choose_random_word();
    len  = strlen(word);

    /* Fill progress with underscores — one for each letter */
    for (i = 0; i < len; i++) {
        progress[i] = '_';
    }
    progress[len] = '\0';   /* null terminate the array */

    printf("\n  A new word has been chosen with %d letters.\n\n", len);

    /* Temporary guessing loop — just to test display and reveal.
       Proper game logic with win/lose and hangman added next commit. */
    while (1) {
        display_word_progress(word, progress);

        printf("\n  Guess a letter: ");
        scanf(" %c", &letter);
        clear_input_buffer();

        /* Basic letter validation */
        if (letter < 'a' || letter > 'z') {
            printf("  Please enter a lowercase letter (a-z).\n\n");
            continue;
        }

        /* Check the guess and give feedback */
        if (update_progress(word, progress, letter)) {
            printf("  Good guess! '%c' is in the word.\n\n", letter);
        } else {
            printf("  Wrong! '%c' is not in the word.\n\n", letter);
        }

        /* Temporary: show the word to confirm reveal is working */
        printf("  [DEBUG] Full word: %s\n\n", word);
    }

    return 0;
}