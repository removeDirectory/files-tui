#include "../../include/file-tree.h"
#include "../../include/stdafx.h"
#include <stdlib.h>
#include <stdint.h>

struct FileTreeNode
{
  char *nodeName;
  size_t nodeNameLength;
  enum FileObjType nodeType;
  struct FileTreeNode *children;
  size_t childrenCount;
  uint64_t fileSize;
  uint64_t createdAt;
  uint64_t lastModifiedAt;
};

enum FileObjType
{
  File,
  Directory,
  Link,
  Other
};

/*
Assemble a tree containing the file structure
*/
struct FileTreeNode *CreateTree()
{
}