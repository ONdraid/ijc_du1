/**
 * @file error.h
 * @brief Header file for error.c.
 * @author Ondřej Vomáčka (xvomaco00), FIT
 * @date 15/03/2024
 *
 * @note First IJC homework part B) solution.
 * @note Compiled with GCC 11.4.0.
 */

#ifndef IJC_DU1_ERROR_H
#define IJC_DU1_ERROR_H

extern void warning(const char *fmt, ...);

extern void error_exit(const char *fmt, ...);

#endif //IJC_DU1_ERROR_H
