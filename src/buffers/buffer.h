#pragma once

#include <sys/types.h>

struct Buffer {
 public:
  size_t size;
  int from_fd;
  int to_fd;
  char data[4096];
};