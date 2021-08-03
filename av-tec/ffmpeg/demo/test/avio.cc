#include <iostream>

extern "C" {
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
}

int get_proc_name() {
  std::cout << avio_find_protocol_name("http://0.0.0.0:/test.mp4") << std::endl;
  return 0;
}

int main() {
  get_proc_name();
  return 0;
}