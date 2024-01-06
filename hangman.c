#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "cs136-trace.h"
#include "words.h"
#include "hangman-io.h"

// global constants
const int LEN_GUESSED_LETTER = 55;  // only takes uppercase letters (26) - use limit all letters just in case (52)
const int MAX_PHRASE_LEN = 51;      // 50 char + 1 '/0'

// is_contained(letter, arr, len) takes a letter, an array with length len and
//      returns true if the letter occurs in the arr and false otherwise
// require: letter is an uppercase letter 
//          arr is a valid pointer
//          arr is a pointer to an array with ALL uppercase letters (not asserted)
//          len >= 0
//          arr has length len (not asserted)
// time: O(n) where n = len
bool is_contained(const char letter, const char *arr, int len){
    assert(arr);
    assert(len >= 0);
    assert(letter >= 'A' && letter <= 'Z');

    for (int i = 0; i < len; i++){
        if (letter == arr[i]) {
            return true;
        }
    }
    return false;
}

// print_word(s, letter_guessed, len) takes a string s, an array of guessed characters with
//      length len, and prints space for spaces in s, letter for characters contained in 
//      the array, and * for other characters
// require: s is a valid pointer
//          letter_guessed is a valid pointer
//          len >= 0
//          s is a string with uppercase letters or spaces (not asserted)
//          arr is a pointer to an array with ALL uppercase letters (not asserted)
//          arr has length len (not asserted)
// effect: generates outputs
// time: O(mn) 
//      where m is the length of the string 
//      n = len
void print_word(const char *s, const char *letter_guessed, int len){
    assert(s);
    assert(len >= 0);

    int i = 0;
    while (s[i]) {
        if (s[i] == ' ') {
            printf("%c", ' ');
        } else if (is_contained(s[i], letter_guessed, len)) {
            printf("%c", s[i]);
        } else {
            printf("%c", '*');
        }
        i++;
    }
    printf("\n");
}

// get_uppercase(letter) gets the uppercase version of letter if it is a lowercase letter
//      and returns it otherwise
// requires: letter is a printable character
// time: O(1)
char get_uppercase(char letter){
    assert(' ' <= letter && letter <= '~');
    if (letter >= 'a' && letter <= 'z') {
        return letter - 'a' + 'A';
    } else {
        return letter;
    }
}

// is_game_won(s, letter_guessed, len) checks if all letters in s is contained in letter_guessed
//      returns true if so, and false otherwise
// require: s is a valid pointer
//          letter_guessed is a valid pointer
//          len >= 0
//          s is a string with uppercase letters or spaces (not asserted)
//          arr is a pointer to an array with ALL uppercase letters (not asserted)
//          arr has length len (not asserted)
// time: O(mn) 
//      where m is the length of the string 
//      n = len
bool is_game_won(const char *s, const char *letter_guessed, int len){
    assert(s);
    assert(len >= 0);

    int i = 0;
    while (s[i]) {
        if (s[i] == ' ') {
        } else if (!is_contained(s[i], letter_guessed, len)) {
            return false;
        }
        i++;
    }
    return true;
}

// reset_arr(arr, len) sets all the elements in the array to 0
// requires: arr is a valid pointer
//           len >= 0
//          arr has length len (not asserted)
// effect: modify arr
// time: O(n)
void reset_arr(char *arr, int len){
    for (int i = 0; i < len; i++){
        arr[i] = 0;
    }
}

int main(void) {
    int phrase_index_N = 0;
    char phrase[51];        // maximum phrase length = 51 (global constants)
    const char * original_phrase;
    int strike = 0;
    char letter_input = 0;
    int phrase_len = 0;
    char letter_guessed[55];        // maximum num of guesses = 55 (global constants)
    int insert_guess_index = 0;

    bool in_game = false;
    bool quit = false;

    while (!quit) {
        // reset
        reset_arr(letter_guessed, LEN_GUESSED_LETTER);
        reset_arr(phrase, MAX_PHRASE_LEN);
        phrase_index_N = 0;
        strike = 0;
        letter_input = 0;
        phrase_len = 0;
        insert_guess_index = 0;
        bool entering_letter = false;

        // asks to choose a word
        printf("Please enter a valid game number between 1 and 1000.\n");
        // if the entered number is not in range - repeats the process until a valid number is entered
        while (scanf("%d", &phrase_index_N) != 1 || phrase_index_N <= 0 
                || phrase_index_N > max_words) {
            printf("Error, the number entered was not valid.\n");
            printf("Please enter a valid game number between 1 and 1000.\n");
        }
        // get word as constant pointer based on N
        original_phrase = get_word(phrase_index_N);
        // copy word to a mutable string
        phrase_len = strlen(original_phrase);
        // converts the string entirely to Upper case
        for (int i = 0; i < phrase_len; i++) {
            phrase[i] = get_uppercase(original_phrase[i]);
        }
        // attach the '\0' at the end to terminate the string
        phrase[phrase_len] = original_phrase[phrase_len];

        // enters the game loop
        in_game = true;

        while (in_game){
            // reset input value
            letter_input = 0;

            // prints hangman
            print_hangman(strike);
            printf("Please enter a letter.\n");
            print_word(phrase, letter_guessed, LEN_GUESSED_LETTER);
            
            // gets a letter input that has not been entered
            entering_letter = true;
            while (entering_letter) {
                // if the entered character is not a valid letter - repeats the process until a valid input entered
                while (scanf(" %c", &letter_input) != 1 
                        || letter_input < 'A' 
                        || (letter_input > 'Z' && letter_input < 'a')
                        || letter_input > 'z') {
                    printf("The character %c is not a letter.\n", letter_input); 
                    print_hangman(strike);
                    printf("Please enter a letter.\n");
                    print_word(phrase, letter_guessed, LEN_GUESSED_LETTER);
                }
                // converts the input letter to upper case - so we ignore lower cases afrer
                letter_input = get_uppercase(letter_input);
                // if the letter has been entered,  repeats the loop and get another letter
                if (is_contained(letter_input, letter_guessed, LEN_GUESSED_LETTER)) {
                    printf("You have already guessed the letter %c. Please enter another letter.\n",
                        letter_input);
                    print_hangman(strike);
                    printf("Please enter a letter.\n");
                    print_word(phrase, letter_guessed, LEN_GUESSED_LETTER);
                } else {    // if the letter entered is valid and not repeated
                    letter_guessed[insert_guess_index] = letter_input;
                    insert_guess_index++;
                    entering_letter = false;  // breaks out of the input loop
                }
            }

            // if the letter is contained in the phrase
            if (is_contained(letter_input, phrase, phrase_len)) {
                // if all letters in the phrase have been guessed - the player wins
                if (is_game_won(phrase, letter_guessed, LEN_GUESSED_LETTER)) {
                    printf("You win! The correct word was %s.\n", phrase);
                    printf("Do you want to play again? (Y for yes, N for no).\n");
                    // asks if the player wants to play again until a valid input is entered
                    while (scanf(" %c", &letter_input) != 1 || 
                        (letter_input != 'N' && letter_input != 'n' && letter_input != 'Y' && letter_input != 'y')) {
                            printf("Error, invalid response.\n");
                            printf("Do you want to play again? (Y for yes, N for no).\n");
                        }
                    // if the player does not want to play again - terminates the quit (outer most loop) and terminates program
                    if (letter_input == 'N' || letter_input == 'n') {
                        in_game = false;
                        quit = true;
                    } else {   
                        // if the player wants to start a new game - breaks out of in_game (first inner loop) so variables
                        //    are reset but the program still runs
                        in_game = false;
                    }
                } else {
                    // if the player has not won - prints hangman at the start of the loop and game continues
                }
            // if the letter is not contained in the phrase (wrong guess)
            } else {
                // adds strike
                strike += 1;
                printf("The letter %c is not in the word.\n", letter_input);
                // if maximum strikes reached - player lost
                if (strike >= max_strikes) {
                    print_hangman(strike);
                    printf("Game over. The correct word was %s.\n", phrase);
                    printf("Do you want to play again? (Y for yes, N for no).\n");
                    // asks if the player wants to play again until a valid input is entered
                    while (scanf(" %c", &letter_input) != 1 || 
                        (letter_input != 'N' && letter_input != 'n' && letter_input != 'Y' && letter_input != 'y')) {
                            printf("Error, invalid response.\n");
                            printf("Do you want to play again? (Y for yes, N for no).\n");
                        }
                    // if the player does not want to play again - terminates the quit (outer most loop) and terminates program
                    if (letter_input == 'N' || letter_input == 'n') {
                        in_game = false;
                        quit = true;
                    } else {
                        // if the player wants to start a new game - breaks out of in_game (first inner loop) so variables
                        //    are reset but the program still runs
                        in_game = false;
                    }
                } else {
                    // warns the player strikes and prints hangman at the start of the loop and game continues
                    printf("Watch out! You only have %d more guesses left before your man is hung!\n",
                        max_strikes - strike);
                }
            }
        }
    }

}
