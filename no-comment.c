/**
 * @file no-comment.c
 * @brief Implementation of comment removal tool.
 * @author Ondřej Vomáčka (xvomaco00)
 * @date 15/03/2024
 *
 * @note First IJC homework part B) solution.
 * @note Compiled with GCC 11.4.0.
 */

#define _POSIX_C_SOURCE 200809L // For fileno().

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "error.h"

// THE STATE DEFINITIONS FOR THE STATE MACHINE

#define STATE_NO_COMMENT 0                      // No comment is being processed.
#define STATE_SINGLE_LINE_COMMENT 1             // Single-line comment is being processed.
#define STATE_SINGLE_LINE_COMMENT_MAYBE 2       // Maybe single-line comment is being processed.
#define STATE_MULTI_LINE_COMMENT 3              // Multi-line comment is being processed.
#define STATE_MULTI_LINE_COMMENT_MAYBE 4        // Maybe multi-line comment is being processed.
#define STATE_MULTI_LINE_COMMENT_END_MAYBE 5    // Maybe the end of multi-line comment is being processed.
#define STATE_INSIDE_QUOTES 6                   // Insides of quotes are being processed.
#define STATE_INSIDE_QUOTES_ESCAPED 7           // Escaped character inside quotes is being processed.
#define STATE_INSIDE_SINGLE_QUOTES 8            // Insides of single quotes are being processed.
#define STATE_INSIDE_SINGLE_QUOTES_ESCAPED 9    // Escaped character inside single quotes is being processed.

/**
 * @brief Checks the arguments and exits with an error message if invalid.
 *
 * @param argc Number of arguments.
 * @param argv Array of arguments.
 */
void check_args(const int argc, const char **argv)
{
    // Just a sanity check, normally this should not happen.
    if (argc == 0) error_exit("unexpected arguments provided.");

    if (argc > 2) error_exit("too many arguments provided.\nUsage: %s [file]\n", argv[0]);
}

int main(const int argc, const char **argv)
{
    // Check validity of arguments.
    check_args(argc, argv); // This can exit the program!

    // Contents of the file if provided, otherwise contents of stdin.
    FILE *stream = stdin;

    if (argc == 2)
    {
        stream = fopen(argv[1], "r");
        if (stream == NULL) error_exit("cannot open file \"%s\".\n", argv[1]);

        struct stat file_stat, output_stat;

        // Check if stdout is redirected to the input file and exit if so.
        fstat(fileno(stream), &file_stat);
        fstat(fileno(stdout), &output_stat);
        if (file_stat.st_dev == output_stat.st_dev && file_stat.st_ino == output_stat.st_ino)
        {
            fclose(stream);
            error_exit("the output is redirected to the input file.\n");
        }
    }

    // COMMENT REMOVAL STATE MACHINE

    int c; // Current character being processed.
    int num_of_stars = 0; // Number of stars in "multi-line comment".
    int state = STATE_NO_COMMENT;
    while ((c = getc(stream)) != EOF) {
        switch (state) {
            // Print everything until we find an indication of a comment.
            case STATE_NO_COMMENT:
                if (c == '/')
                {
                    // Following character will determine the type of comment, if any.
                    state = STATE_SINGLE_LINE_COMMENT_MAYBE;
                    break;
                }
                else if (c == '"') state = STATE_INSIDE_QUOTES; // We are inside quotes.
                else if (c == '\'') state = STATE_INSIDE_SINGLE_QUOTES; // We are inside single quotes.
                putchar(c);
                break;

            // Determine the type of comment, if any.
            case STATE_SINGLE_LINE_COMMENT_MAYBE:
                if (c == '/')
                {
                    // We are sure it is a one-line comment.
                    state = STATE_SINGLE_LINE_COMMENT;
                    break;
                }
                else if (c == '*')
                {
                    // Following characters will determine if it is a multi-line comment.
                    state = STATE_MULTI_LINE_COMMENT_MAYBE;
                    break;
                }
                else if (c == '"') state = STATE_INSIDE_QUOTES; // We are inside quotes.
                else if (c == '\'') state = STATE_INSIDE_SINGLE_QUOTES; // We are inside single quotes.
                putchar('/'); // It was not a comment, so we print the previous backslash.
                putchar(c); // And the current character.
                state = STATE_NO_COMMENT;
                break;

            // Skip all characters until the end of the line.
            case STATE_SINGLE_LINE_COMMENT:
                if (c == '\n')
                {
                    state = STATE_NO_COMMENT;
                    putchar('\n');
                    break;
                }
                break;

            // Determine if it is a multi-line comment.
            case STATE_MULTI_LINE_COMMENT_MAYBE:
                if (c == '/')
                {
                    // It was not a multi-line comment, so we print the original characters.
                    printf("/*");
                    for (int i = 0; i < num_of_stars; ++i) putchar('*');
                    printf("/");
                    state = STATE_NO_COMMENT;
                    num_of_stars = 0;
                    break;
                }
                else if (c != '*')
                {
                    // We are sure it is a multi-line comment.
                    state = STATE_MULTI_LINE_COMMENT;
                    num_of_stars = 0;
                    break;
                }
                // Increment the number of stars.
                // This will be needed to print the original characters back if it is not a multi-line comment.
                num_of_stars++;
                break;

            // Ignore all characters until star is found.
            case STATE_MULTI_LINE_COMMENT:
                if (c == '*') state = STATE_MULTI_LINE_COMMENT_END_MAYBE;
                break;

            // Check if the multi-line comment is ending.
            case STATE_MULTI_LINE_COMMENT_END_MAYBE:
                if (c == '/')
                {
                    // It is actually the end of multi-line comment.
                    state = STATE_NO_COMMENT;
                    break;
                }
                // It was just a star! No need to panic!
                state = STATE_MULTI_LINE_COMMENT;
                break;

            // Ignore all possible comments inside quotes and print everything.
            case STATE_INSIDE_QUOTES:
                if (c == '\\') state = STATE_INSIDE_QUOTES_ESCAPED; // Skip the next character.
                else if (c == '"') state = STATE_NO_COMMENT; // We are outside of quotes.
                putchar(c);
                break;

            // Skip the next character and return to "inside quotes" state.
            case STATE_INSIDE_QUOTES_ESCAPED:
                state = STATE_INSIDE_QUOTES;
                putchar(c);
                break;

            // Ignore all quotes and possible comments inside single quotes and print everything.
            case STATE_INSIDE_SINGLE_QUOTES:
                if (c == '\\') state = STATE_INSIDE_SINGLE_QUOTES_ESCAPED; // Skip the next character.
                else if (c == '\'') state = STATE_NO_COMMENT; // We are outside single quotes.
                putchar(c);
                break;

            // Skip the next character and return to "inside single quotes" state.
            case STATE_INSIDE_SINGLE_QUOTES_ESCAPED:
                state = STATE_INSIDE_SINGLE_QUOTES;
                putchar(c);
                break;
        }
    }

    if (argc == 2) fclose(stream); // Close the file if it was opened.

    // Exit with if the file ends with incomplete comment or string.
    if (state != STATE_NO_COMMENT &&
        state != STATE_SINGLE_LINE_COMMENT_MAYBE &&
        state != STATE_MULTI_LINE_COMMENT_MAYBE)
        error_exit("an incomplete comment or string at the end of the file.\n");

    return 0;
}
