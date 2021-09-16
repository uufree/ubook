#include <iostream>
#include "avio.h"
#include "decode.h"
#include "audio.h"
#include "st_audio_extract.h"

int main() {
  int ret = 0;

  AvIODemo demo;
  demo.avio_malloc_free();
  demo.read_from_memory("/Users/chenyouyou1/ubook/av-tec/ffmpeg/demo/data/terror.mp4");

  return 0;
}
