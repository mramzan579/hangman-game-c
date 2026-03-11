/*
 * hangman.c
 *
 * A console-based Hangman game written in C.
 * The computer picks a random word and the player guesses
 * one letter at a time before running out of attempts.
 *
 * Compile : gcc hangman.c -o hangman
 * Run     : ./hangman
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ── Constants ──────────────────────────────────────────────── */
#define WORD_COUNT    10
#define MAX_WRONG      6
#define MAX_WORD_LEN  20

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
   Prevents input bugs when reading single characters.
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
   srand() is seeded once in main() before this is called.
   Returns a pointer to the chosen word string.
================================================================ */
const char *choose_random_word(void)
{
    int index = rand() % WORD_COUNT;
    return word_bank[index];
}

/* ================================================================
   display_word_progress()
   Prints the current state of the word — showing correctly
   guessed letters and underscores for unguessed positions.
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
   Checks if the guessed letter exists anywhere in the word.
   Reveals it in the progress array at all matching positions.
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
   Prints the ASCII hangman figure based on wrong guess count.
   Builds step by step — one body part per wrong guess:
     wrong 1 — head
     wrong 2 — body
     wrong 3 — left arm
     wrong 4 — right arm
     wrong 5 — left leg
     wrong 6 — right leg (full figure = game over)
   Parameters:
     wrong — number of incorrect guesses so far
================================================================ */
void draw_hangman(int wrong)
{
    printf("\n  +---+\n");
    printf("  |   %s\n",  wrong >= 1 ? "O" : " ");
    printf("  |   %s\n",  wrong >= 2 ? "|" : " ");

    if      (wrong >= 4) printf("  |  /|\\\n");
    else if (wrong >= 3) printf("  |  /|\n");
    else                 printf("  |   \n");

    if      (wrong >= 6) printf("  |  / \\\n");
    else if (wrong >= 5) printf("  |  /\n");
    else                 printf("  |   \n");

    printf("  |\n");
    printf(" ===\n\n");
}

/* ================================================================
   ask_play_again()
   Asks the player if they want another round.
   Validates input — only accepts Y or N.
   Returns 1 for yes, 0 for no.
================================================================ */
int ask_play_again(void)
{
    char choice;

    while (1) {
        printf("Play again? (Y/N): ");
        scanf(" %c", &choice);
        clear_input_buffer();

        if (choice == 'y' || choice == 'Y') return 1;
        if (choice == 'n' || choice == 'N') return 0;

        printf("  Please enter Y or N.\n");
    }
}

/* ================================================================
   play_game()
   Runs one complete round of Hangman from start to finish.
   - Picks a random word
   - Fills progress array with underscores
   - Loops until player wins or runs out of attempts
   - Tracks guessed letters to reject duplicates
   - Checks win condition after every correct guess
================================================================ */
void play_game(void)
{
    const char *word;
    char        progress[MAX_WORD_LEN];
    char        guessed[26];
    int         guessed_count = 0;
    int         wrong         = 0;
    int         i, len;
    char        letter;
    int         already_guessed;
    int         correct_count;

    /* Pick a word and build the progress array */
    word = choose_random_word();
    len  = strlen(word);

    for (i = 0; i < len; i++) progress[i] = '_';
    progress[len] = '\0';

    printf("\n  A new word has been chosen with %d letters.\n", len);
    printf("  You have %d wrong guesses before game over.\n\n", MAX_WRONG);

    /* ── Main guessing loop ──────────────────────────────────── */
    while (wrong < MAX_WRONG) {

        draw_hangman(wrong);
        display_word_progress(word, progress);
        printf("  Wrong guesses left: %d\n", MAX_WRONG - wrong);

        /* Show all letters tried so far */
        if (guessed_count > 0) {
            printf("  Letters tried: ");
            for (i = 0; i < guessed_count; i++) {
                printf("%c ", guessed[i]);
            }
            printf("\n");
        }

        /* Get a letter from the player */
        printf("\n  Guess a letter: ");
        scanf(" %c", &letter);
        clear_input_buffer();

        /* Only lowercase letters allowed */
        if (letter < 'a' || letter > 'z') {
            printf("  Please enter a lowercase letter (a-z).\n\n");
            continue;
        }

        /* Reject duplicate guesses */
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

        /* Check the guess and give feedback */
        if (update_progress(word, progress, letter)) {
            printf("  Good guess! '%c' is in the word.\n\n", letter);
        } else {
            printf("  Wrong! '%c' is not in the word.\n\n", letter);
            wrong++;
        }

        /* ── Check win condition ─────────────────────────────── */
        /* Count how many positions are still underscores */
        correct_count = 0;
        for (i = 0; i < len; i++) {
            if (progress[i] != '_') correct_count++;
        }

        /* If no underscores remain — player has won */
        if (correct_count == len) {
            draw_hangman(wrong);
            display_word_progress(word, progress);
            printf("  YOU WIN! Well done!\n");
            printf("  The word was: %s\n\n", word);
            return;   /* exit play_game — round is over */
        }
    }

    /* ── Lose condition ──────────────────────────────────────── */
    /* Loop ended because wrong reached MAX_WRONG */
    draw_hangman(wrong);
    display_word_progress(word, progress);
    printf("  GAME OVER! You ran out of guesses.\n");
    printf("  The word was: %s\n\n", word);
}

/* ================================================================
   main()
   Seeds the random number generator once, prints the welcome
   banner, then runs the game in a loop until the player quits.
================================================================ */
int main(void)
{
    /* Seed once at the very start */
    srand((unsigned int)time(NULL));

    /* Welcome banner */
    printf("========================================\n");
    printf("            HANGMAN GAME                \n");
    printf("========================================\n");
    printf("  Guess the hidden word one letter at a time.\n");
    printf("  You have %d wrong guesses before game over.\n", MAX_WRONG);
    printf("========================================\n");

    /* Main replay loop */
    do {
        play_game();
    } while (ask_play_again());

    /* Final goodbye */
    printf("\n========================================\n");
    printf("  Thanks for playing! Goodbye.\n");
    printf("========================================\n\n");

    return 0;
}