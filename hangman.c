/*
 * hangman.c
 * Hangman Game in C
 * Add the ASCII hangman drawing, wrong attempt counter,
 * and duplicate letter detection.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Constants 
#define WORD_COUNT    10
#define MAX_WRONG      6
#define MAX_WORD_LEN  20

// Word bank
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
   Prints the current state of the word — letters or underscores.
   Parameters:
     word     — the secret word
     progress — holds guessed letters or '_' for unguessed spots
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
   Checks if the guessed letter exists in the word.
   Reveals it in progress array at all matching positions.
   Returns 1 if found, 0 if not found.
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
            progress[i] = letter;
            found = 1;
        }
    }
    return found;
}

/* ================================================================
   draw_hangman()
   Prints the ASCII hangman figure step by step.
   Each body part appears when wrong reaches its threshold:
     wrong 1 — head
     wrong 2 — body
     wrong 3 — left arm
     wrong 4 — right arm
     wrong 5 — left leg
     wrong 6 — right leg (game over)
   Parameters:
     wrong — number of incorrect guesses so far
================================================================ */
void draw_hangman(int wrong)
{
    printf("\n  +---+\n");

    /* Head appears on wrong guess 1 */
    printf("  |   %s\n", wrong >= 1 ? "O" : " ");

    /* Body appears on wrong guess 2 */
    printf("  |   %s\n", wrong >= 2 ? "|" : " ");

    /* Arms — left on guess 3, both on guess 4 */
    if      (wrong >= 4) printf("  |  /|\\\n");
    else if (wrong >= 3) printf("  |  /|\n");
    else                 printf("  |   \n");

    /* Legs — left on guess 5, both on guess 6 */
    if      (wrong >= 6) printf("  |  / \\\n");
    else if (wrong >= 5) printf("  |  /\n");
    else                 printf("  |   \n");

    printf("  |\n");
    printf(" ===\n\n");
}

//── Main 
int main(void)
{
    const char *word;
    char        progress[MAX_WORD_LEN];
    char        guessed[26];      /* stores every letter tried so far */
    int         guessed_count = 0;
    int         wrong         = 0;
    int         i, len;
    char        letter;
    int         already_guessed;

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

    for (i = 0; i < len; i++) progress[i] = '_';
    progress[len] = '\0';

    printf("\n  A new word has been chosen with %d letters.\n\n", len);

    // Main guessing loop 
    while (wrong < MAX_WRONG) {

        /* Draw the hangman figure for current wrong count */
        draw_hangman(wrong);

        /* Show the word progress */
        display_word_progress(word, progress);

        /* Show remaining wrong guesses */
        printf("  Wrong guesses left: %d\n", MAX_WRONG - wrong);

        /* Show all letters tried so far */
        if (guessed_count > 0) {
            printf("  Letters tried: ");
            for (i = 0; i < guessed_count; i++) {
                printf("%c ", guessed[i]);
            }
            printf("\n");
        }

        // Get a letter from the player
        printf("\n  Guess a letter: ");
        scanf(" %c", &letter);
        clear_input_buffer();

        /* Validate — only lowercase letters allowed */
        if (letter < 'a' || letter > 'z') {
            printf("  Please enter a lowercase letter (a-z).\n\n");
            continue;
        }

        /* Check if this letter was already guessed */
        already_guessed = 0;
        for (i = 0; i < guessed_count; i++) {
            if (guessed[i] == letter) {
                already_guessed = 1;
                break;
            }
        }
        if (already_guessed) {
            printf("  You already tried '%c'. Try a different letter.\n\n",
                   letter);
            continue;
        }

        /* Record this letter as tried */
        guessed[guessed_count++] = letter;

        /* Check the guess */
        if (update_progress(word, progress, letter)) {
            printf("  Good guess! '%c' is in the word.\n\n", letter);
        } else {
            printf("  Wrong! '%c' is not in the word.\n\n", letter);
            wrong++;   /* only increments on wrong guess */
        }
    }

    /* Temporary end — win/lose messages and replay added next commit */
    printf("\n  [DEBUG] Game ended. Word was: %s\n\n", word);

    return 0;
}