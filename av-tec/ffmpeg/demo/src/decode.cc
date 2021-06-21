#include <iostream>
#include "decode.h"
#include "defer.h"

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

int decode(const char *media_path, DecodeType type) {
  AVFormatContext *input_ctx = nullptr;
  AVCodecContext *codec_ctx = nullptr;
  AVCodec *codec = nullptr;
  AVPacket *packet = nullptr;
  AVFrame *frame = nullptr;
  int stream_index = -1;
  char ffmpeg_errstr[256];
  int ret = 0;

  if (!(input_ctx = avformat_alloc_context())) {
    std::cerr << "alloc format ctx failed" << std::endl;
    return -1;
  }
  defer(avformat_free_context(input_ctx));

  if ((ret = avformat_open_input(&input_ctx, media_path, nullptr, nullptr)) < 0) {
    std::cerr << "open url failed, ret: " << ret << std::endl;
    return -2;
  }
  defer(avformat_close_input(&input_ctx));

  if ((ret = avformat_find_stream_info(input_ctx, nullptr)) < 0) {
    std::cerr << "find stream info failed, ret: " << ret << std::endl;
    return -3;
  }

  AVMediaType media_type = type == Audio ? AVMEDIA_TYPE_AUDIO : AVMEDIA_TYPE_VIDEO;
  for (int i = 0; i < input_ctx->nb_streams; i++) {
    if (input_ctx->streams[i]->codecpar->codec_type == media_type) {
      stream_index = i;
      break;
    }
  }

  if (stream_index == -1) {
    std::cerr << "not find stream index" << std::endl;
    return -4;
  }

  if (!(codec = avcodec_find_decoder(input_ctx->streams[stream_index]->codecpar->codec_id))) {
    std::cerr << "find acc decoder failed" << std::endl;
    return -5;
  }

  if (!(codec_ctx = avcodec_alloc_context3(codec))) {
    std::cerr << "alloc codec context failed" << std::endl;
    return -6;
  }
  defer(avcodec_free_context(&codec_ctx));

  if ((ret = avcodec_parameters_to_context(codec_ctx, input_ctx->streams[stream_index]->codecpar)) < 0) {
    std::cerr << "copy param to context failed, ret: " << ret << std::endl;
    return -7;
  }

  if ((ret = avcodec_open2(codec_ctx, codec, nullptr)) < 0) {
    std::cerr << "open codec ctx failed, ret: " << ret << std::endl;
    return -7;
  }
  defer(avcodec_close(codec_ctx));

  if (!(packet = av_packet_alloc())) {
    std::cerr << "alloc packet failed" << std::endl;
    return -8;
  }
  defer(av_packet_free(&packet));

  if (!(frame = av_frame_alloc())) {
    std::cerr << "alloc frame failed" << std::endl;
    return -9;
  }
  defer(av_frame_free(&frame));

  while (av_read_frame(input_ctx, packet) >= 0) {
    defer(av_packet_unref(packet));

    if (packet->stream_index != stream_index) {
      continue;
    }

    if ((ret=avcodec_send_packet(codec_ctx, packet)) < 0) {
      std::cerr << "call send packet failed, ret: " << ret << std::endl;
      return -10;
    }

    while (ret >= 0) {
      if ((ret = avcodec_receive_frame(codec_ctx, frame)) < 0) {
        if (ret == AVERROR_EOF || ret == AVERROR(EAGAIN)) {
          continue;
        }

        std::cerr << "call avcodec_receive_frame failed, ret: " << ret << std::endl;
        return -11;
      }

      defer(av_frame_unref(frame));
      std::cout << frame->pts << "    " << frame->pkt_size << std::endl;
    }
  }

  // flush
  if ((ret = avcodec_send_packet(codec_ctx, nullptr)) < 0) {
    std::cerr << "flush: call send packet failed, ret: " << ret << std::endl;
    return -12;
  }

  while (ret >= 0) {
    if ((ret = avcodec_receive_frame(codec_ctx, frame)) < 0) {
      if (ret == AVERROR_EOF || ret == AVERROR(EAGAIN)) {
        continue;
      }

      std::cerr << "call avcodec_receive_frame failed, ret: " << ret << std::endl;
      return -10;
    }

    defer(av_frame_unref(frame));
    std::cout << "flush: " << frame->pts << "    " << frame->pkt_size << std::endl;
  }


  return 0;
}