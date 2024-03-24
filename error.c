/**
 * @file error.c
 * @brief Implementation of warning / error printing functions.
 * @author Ondřej Vomáčka (xvomaco00), FIT
 * @date 15/03/2024
 *
 * @note First IJC homework part B) solution.
 * @note Compiled with GCC 11.4.0.
 */

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * @brief Prints a message to stderr with a given prefix.
 *
 * @param prefix The string to be printed before message. E.g. if "Warning" -> "Warning: {message}".
 * @param fmt The format string. Just like printf()'s format string.
 * @param args The arguments for the format string.
 */
void print_to_stderr_with_prefix(const char *prefix, const char *fmt, va_list args)
{
    fprintf(stderr, "%s: ", prefix); // Print the prefix.
    vfprintf(stderr, fmt, args); // Print the message.
}

/**
 * @brief Prints a warning message to stderr.
 *
 * @param fmt The format string. Just like printf()'s format string.
 * @param args The arguments for the format string.
 */
void warning(const char *fmt, ...)
{
    va_list args; va_start(args, fmt);
    print_to_stderr_with_prefix("WARNING", fmt, args);
    va_end(args);
}

/**
 * @brief Prints an error message to stderr and exits.
 *
 * @param fmt The format string. Just like printf()'s format string.
 * @param args The arguments for the format string.
 */
void error_exit(const char *fmt, ...)
{
    va_list args; va_start(args, fmt);
    print_to_stderr_with_prefix("ERROR", fmt, args);
    va_end(args);
    exit(EXIT_FAILURE);
}
