// Global Imports

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef _WIN32
#include <windows.h>
#endif

// Constant Declarations

#ifndef GLOBAL
#define GLOBAL

typedef enum
{
  File,
  Directory,
  Link,
  Other
} FileType;

typedef struct
{
  const char *name;
  FileType type;
  uint64_t fileSize;
  time_t createdAt;
  time_t lastModifiedAt;
} FileData;

#ifdef _WIN32
char FILE_SEPARATOR = '\\';
#else
char FILE_SEPARATOR = '/';
#endif

#endif
