#ifndef AVIO_H
#define AVIO_H

class AvIODemo {
 public:
  int avio_malloc_free();
  int read_and_write();
  int read_from_memory(const char *video_path);
 private:
  static const int errbuf_size = 64;
  static char errbuf[errbuf_size];
};

#endif