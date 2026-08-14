#include "../../include/stdafx.h"
#include "../../include/filedata.h"
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>

/*
getItemsInDir takes a path and an empty pointer, and reads into
`arr`.
Returns the number of entries in `arr`.
There is no difference in behaviour for an empty directory, or a file.
*/
size_t getItemsInDir(const char *path, FileData **arr)
{
  struct dirent *de;

  DIR *dir = opendir(path);
  if (dir == NULL)
  {
    return 0;
  }

  char fullpath[2048];
  size_t itemCount = 0;

  while ((de = readdir(dir)) != NULL)
  {
    sprintf_s(fullpath, sizeof(fullpath), "%s%c%s", path, FILE_SEPARATOR, de->d_name);

    itemCount++;
    arr = realloc(arr, itemCount * sizeof(FileData *));
    arr[itemCount - 1] = parseFile(de->d_name, fullpath);
  }

  closedir(dir);

  return itemCount;
}

#ifdef _WIN32

FileData *parseFile(const char *filename, const char *path)
{
  struct _stat64 fileInfo;

  _stat64(path, &fileInfo);

  FileData data;
  data.name = filename;
  data.fileSize = fileInfo.st_size;
  data.createdAt = fileInfo.st_ctime;
  data.lastModifiedAt = fileInfo.st_mtime;

  if (fileInfo.st_mode & _S_IFDIR > 0)
  {
    data.type = Directory;
    return &data;
  }
  else if (fileInfo.st_mode & _S_IFREG <= 0)
  {
    data.type = Other;
    return &data;
  }

  WIN32_FIND_DATA ffd;
  HANDLE h = FindFirstFile(path, &ffd);

  if (ffd.dwReserved0 & IO_REPARSE_TAG_SYMLINK > 0)
  {
    data.type = Link;
  }
  else
  {
    data.type = File;
  }

  return &data;
}

#else

FileData *parseFile(const char *filename, const char *path)
{
}

#endif