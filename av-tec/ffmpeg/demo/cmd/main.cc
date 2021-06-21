#include <iostream>
#include "avio_read.h"
#include "decode.h"

int main() {
  int ret = 0;

//  if ((ret = avio_read("/home/sensetime/ffmpeg/20200904-a.mp4")) < 0) {
//    std::cerr << "call executor failed, ret: " << ret << std::endl;
//    return -1;
//  }

  do
    if ((ret = decode("/home/sensetime/ffmpeg/20200904-a.mp4", Video)) < 0) {
      std::cerr << "call executor failed, ret: " << ret << std::endl;
      return -1;
    }
  while (0);

  return 0;
}
