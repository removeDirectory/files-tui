#include "../include/src.h"
#include "../include/stdafx.h"
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

void output()
{
  initscr();
  clear();
  refresh();
  addstr("peenor");
  refresh();

  getch();
  endwin();
}

enum DirType
{
  file,
  directory,
  link
};

struct DirObject
{
  char *objName;
  size_t nameLen;
  char *fullPath;
  size_t pathLen;
  enum DirType type;
  struct _stat64i32 info;
};

void fileData()
{
  struct dirent *dirEntry;

  DIR *dir = opendir("C:\\Users\\camer\\");
  if (dir == NULL)
  {
    printf("Could not open Windows root dir\n");
    return;
  }

  char buf[255];
  struct _stat64 fileInfo;

  while ((dirEntry = readdir(dir)) != NULL)
  {
    sprintf_s(buf, sizeof(buf), "C:\\%s", dirEntry->d_name);
    _stati64(buf, &fileInfo);
    printf("- %s | %d\n", buf, fileInfo.st_size);
  }

  closedir(dir);
}

#ifdef _WIN32

void parseFile(const char *filename, const char *path, FileData *data)
{
  // i'm going to murder someone at MSFT
  // a) why does everything have to be different to every other platform,
  // and b) why does everything have an 'A' after it???
  data->name = strdup(filename);

  HANDLE h = CreateFileA(
      path,
      GENERIC_READ,
      FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
      NULL,
      OPEN_EXISTING,
      FILE_FLAG_BACKUP_SEMANTICS |      // required for directories
          FILE_FLAG_OPEN_REPARSE_POINT, // don't follow symlinks, open the link itself
      NULL);

  if (h == INVALID_HANDLE_VALUE)
  {
    data->fileSize = 0;
    data->createdAt = 0;
    data->lastModifiedAt = 0;
    data->type = Other;
    return;
  }

  // ── Timestamps ────────────────────────────────────────────────────────────
  FILETIME ft_create, ft_modified;
  GetFileTime(h, &ft_create, NULL, &ft_modified);

  // Convert FILETIME (100ns ticks since 1601-01-01)
  // to time_t (seconds since 1970-01-01)
  ULARGE_INTEGER uli;

  uli.LowPart = ft_create.dwLowDateTime;
  uli.HighPart = ft_create.dwHighDateTime;
  data->createdAt = (time_t)((uli.QuadPart - 116444736000000000ULL) / 10000000ULL);

  uli.LowPart = ft_modified.dwLowDateTime;
  uli.HighPart = ft_modified.dwHighDateTime;
  data->lastModifiedAt = (time_t)((uli.QuadPart - 116444736000000000ULL) / 10000000ULL);

  // ── File size ─────────────────────────────────────────────────────────────
  LARGE_INTEGER size;
  GetFileSizeEx(h, &size);
  data->fileSize = (off_t)size.QuadPart;

  CloseHandle(h);

  DWORD attrs = GetFileAttributesA(path);

  if (attrs == INVALID_FILE_ATTRIBUTES)
  {
    data->type = Other;
    return;
  }

  if (attrs & FILE_ATTRIBUTE_REPARSE_POINT)
  {
    // Confirm it's specifically a symlink and not another reparse type
    // (junctions, OneDrive placeholders, etc. are also reparse points)
    WIN32_FIND_DATAA ffd;
    HANDLE fh = FindFirstFileA(path, &ffd);
    if (fh != INVALID_HANDLE_VALUE)
    {
      FindClose(fh);
      data->type = (ffd.dwReserved0 == IO_REPARSE_TAG_SYMLINK)
                       ? Link
                       : Other;
    }
    else
    {
      data->type = Other;
    }
  }
  else if (attrs & FILE_ATTRIBUTE_DIRECTORY)
  {
    data->type = Directory;
    data->fileSize = 0; // GetFileSizeEx returns 0 for dirs anyway,
                        // call dir_size_recursive() here if you want real size
  }
  else
  {
    data->type = File;
  }
}

#else

void parseFile(const char *filename, const char *path, FileData *data)
{
}

#endif

size_t getItemsInDir(const char *path, FileData ***arr)
{
  struct dirent *de;

  DIR *dir = opendir(path);
  if (dir == NULL)
    return 0;

  char fullpath[2048];
  size_t itemCount = 0;

  while ((de = readdir(dir)) != NULL)
  {
    if (de->d_name[0] == '.')
      continue;

    snprintf(fullpath, sizeof(fullpath), "%s%c%s", path, '\\', de->d_name);

    itemCount++;
    FileData **temp = realloc(*arr, itemCount * sizeof(FileData *));
    if (temp == NULL)
      break;
    *arr = temp;

    FileData *fd = malloc(sizeof(FileData));
    parseFile(de->d_name, fullpath, fd);
    (*arr)[itemCount - 1] = fd;
  }

  closedir(dir);

  return itemCount;
}

void test()
{
  FileData **data = NULL;
  size_t count = getItemsInDir("C:\\Users\\camer\\source\\repos\\1f349\\violet", &data);

  printf_s("count: %d\n", count);

  for (size_t i = 0; i < count; i++)
  {
    printf_s("%s | Type: %d | Size: %dB | Created: %.24s | Modified: %.24s \n", data[i]->name, data[i]->type, data[i]->fileSize, ctime(&data[i]->createdAt), ctime(&data[i]->lastModifiedAt));
  }
}