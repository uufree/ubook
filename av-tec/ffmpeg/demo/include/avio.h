#ifndef AVIO_H
#define AVIO_H

class AvIODemo {
 public:
  int avio_malloc_free();
  int read_and_write();
  int read_from_memory(const char* video_path);
  int avio_read(const char *video_path);
};

#endif