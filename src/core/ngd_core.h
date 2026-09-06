#ifndef NGD_CORE_H
#define NGD_CORE_H
//
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>
//
#define NGD_OK 0
#define NGD_ERR -1
#define NGD_AGAIN -2
//
#define NGD_STATE_START 0
//
#define RED     "\033[31m"
#define RESET   "\033[0m"
//
typedef unsigned char u_char;
//
#endif
