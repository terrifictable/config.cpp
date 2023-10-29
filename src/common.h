/*
 * This is mainly preprocessor macros
 * which only includes some functions for logging at the momenet
 */

#pragma once
#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>


// FIXME: this is only for debugging
#define DEBUG


/* extern "C" guards */
#ifdef __cplusplus
extern "C" {
#endif
/* extern "C" guards */



#ifdef __cplusplus
#define unused [[maybe_unused]]
#else
#define unused __attribute__((unused))
#endif

#define ANSI_RESET "\033[0m"
#define ANSI_GREY  "\033[38;5;242m"
#define ANSI_HIGH  "\033[38;5;159m"

#define ANSI_RED    "\033[38;5;162m"
#define ANSI_GREEN  "\033[38;5;49m"
#define ANSI_BLUE   "\033[38;5;81m"
#define ANSI_ORANGE "\033[38;5;215m"
#define ANSI_YELLOW "\033[38;5;226m"


#define msg(msg, ...)  printf(" " msg "\n", ##__VA_ARGS__)
#define info(msg, ...) printf(" " ANSI_GREY "[" ANSI_BLUE   "*" ANSI_GREY "] " ANSI_RESET msg "\n", ##__VA_ARGS__)

#define ok(msg, ...)   printf(" " ANSI_GREY "[" ANSI_GREEN  "+" ANSI_GREY "] " ANSI_RESET msg "\n", ##__VA_ARGS__)
#define warn(msg, ...) printf(" " ANSI_GREY "[" ANSI_ORANGE "#" ANSI_GREY "] " ANSI_RESET msg "\n", ##__VA_ARGS__)
#define err(msg, ...)  printf(" " ANSI_GREY "[" ANSI_RED    "-" ANSI_GREY "] " ANSI_RESET msg "\n", ##__VA_ARGS__)
#define code_err(msg, ...)  printf(" " ANSI_GREY "[" ANSI_RED    "-" ANSI_GREY "] (%s:%d at %s) " ANSI_RESET msg "\n", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)

#ifdef DEBUG
#define dbg(msg, ...)  printf(" " ANSI_GREY "[" ANSI_YELLOW "~" ANSI_GREY "] " ANSI_RESET msg "\n", ##__VA_ARGS__)
#define dbgf(msg, ...)  printf(" " ANSI_GREY "[" ANSI_YELLOW "~" ANSI_GREY "] " ANSI_RESET msg, ##__VA_ARGS__)
#else
#define dbg(...)
#endif


/* extern "C" guards */
#ifdef __cplusplus
}
#endif
/* extern "C" guards */
#endif /* ifndef COMMON_H */
