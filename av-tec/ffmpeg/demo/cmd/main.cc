#include <iostream>
#include "avio_read.h"
#include "decode.h"
#include "audio.h"
#include "st_audio_extract.h"

int main() {
  int ret = 0;

//  do {
//    if ((ret = avio_read("/home/sensetime/ffmpeg/20200904-a.mp4")) < 0) {
//      std::cerr << "call executor failed, ret: " << ret << std::endl;
//      return -1;
//    }
//  } while (0);

//  do
//    if ((ret = decode("/Users/chenyouyou1/ffmpeg/20200904-a.mp4", Video)) < 0) {
//      std::cerr << "call executor failed, ret: " << ret << std::endl;
//      return -1;
//    }
//  while (0);

//  do {
//    if ((ret = demuxer_and_transcode_aac_to_pcm("/Users/chenyouyou1/ffmpeg/terror.mp4", "/Users/chenyouyou1/Documents/ubook/av-tec/ffmpeg/demo/build/terror.pcm")) < 0) {
//      std::cerr << "call demuxer and transcode to pcm failed. ret: " << ret << std::endl;
//      return -1;
//    }
//  } while (0);

//  do {
//    char errmsg[256];
//    st_audio_extract_init();
//    if ((ret = st_audio_extract("/Users/chenyouyou1/ffmpeg/terror.mp4",
//                                "/Users/chenyouyou1/Documents/ubook/av-tec/ffmpeg/demo/build/terror.pcm",
//                                errmsg)) < 0) {
//      std::cerr << "call st audio extract failed, errmsg: " << errmsg << std::endl;
//      return -1;
//    }
//  } while (0);

  do {
    if ((ret = pcm_to_aac("/Users/chenyouyou1/ffmpeg/terror.pcm", nullptr)) < 0) {
      std::cerr << "call pcm to aac failed" << std::endl;
      return -1;
    }
  } while(0);

  return 0;
}
