#include <iostream>
#include "avio.h"
#include "decode.h"
#include "audio.h"
#include "st_audio_extract.h"

int main() {
  int ret = 0;

  for (int i=0; i<1000; i++) {
    decode("http://10.5.42.118:8081/data/jsyx/2h.mp4", Video);
  }

  return 0;
}
