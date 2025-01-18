// webdev.h
#ifndef WEBDEV_H
#define WEBDEV_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h> // For Windows-specific functionality

typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;
typedef unsigned long long int int64;

#define $c (char *)
#define $8 (int8 *)
#define $6 (int16 *)
#define $2 (int32 *)
#define $4 (int64 *)
#define $i (int)

// Define log levels for Windows
#define LOG_INFO 1
#define LOG_WARNING 2
#define LOG_ERR 3

#define log(x)  syslogger(LOG_INFO, x)
#define logwarning(x)   syslogger(LOG_WARNING, x)
#define logger(x)   syslogger(LOG_ERR, x)

void syslogger(int16 priority, int8 *str);
int32 setupserver(int8 *host, int16 port);
int main(int argc, char **argv);

#endif // WEBDEV_H
