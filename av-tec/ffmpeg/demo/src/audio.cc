#include <iostream>
#include <fstream>
#include "audio.h"
#include "defer.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
}

int demuxer_and_transcode_aac_to_pcm(const char *video_path, const char *pcm_path) {
  AVFormatContext *video_ctx = nullptr;
  AVCodecContext *audio_codec_ctx = nullptr;
  AVCodec *audio_codec = nullptr;
  AVPacket *packet = nullptr;
  AVFrame *frame = nullptr;
  SwrContext *swr_ctx = nullptr;
  uint8_t *swr_out_buffer = nullptr;
  int audio_stream_id = -1;
  int ret = 0;
  int bytes_per_frame = 0;
  std::fstream ofs;

  ofs.open(pcm_path, std::ios_base::binary | std::ios_base::out | std::ios_base::trunc);
  if (!ofs.is_open()) {
    std::cerr << "open pcm data failed" << std::endl;
    return -1;
  }

  if (!(video_ctx = avformat_alloc_context())) {
    std::cerr << "alloc video ctx failed, ret: " << ret << std::endl;
    return -1;
  }
  defer(avformat_free_context(video_ctx));

  if ((ret = avformat_open_input(&video_ctx, video_path, nullptr, nullptr)) < 0) {
    std::cerr << "call format open input failed, ret: " << ret << std::endl;
    return -2;
  }
  defer(avformat_close_input(&video_ctx));

  for (int i = 0; i < video_ctx->nb_streams; i++) {
    if (video_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
      audio_stream_id = i;
      break;
    }
  }

  if (audio_stream_id == -1) {
    std::cerr << "not find audio stream" << std::endl;
    return -3;
  }

  if (!(audio_codec = avcodec_find_decoder(video_ctx->streams[audio_stream_id]->codecpar->codec_id))) {
    std::cerr << "find audio codec failed" << std::endl;
    return -4;
  }

  if (!(audio_codec_ctx = avcodec_alloc_context3(audio_codec))) {
    std::cerr << "alloc audio codec ctx failed" << std::endl;
    return -5;
  }
  defer(avcodec_free_context(&audio_codec_ctx));

  if ((ret = avcodec_parameters_to_context(audio_codec_ctx, video_ctx->streams[audio_stream_id]->codecpar))) {
    std::cerr << "copy audio codec params to codec ctx failed, ret: " << std::endl;
    return -5;
  }

  if ((ret = avcodec_open2(audio_codec_ctx, audio_codec, nullptr)) < 0) {
    std::cerr << "open audio codec ctx failed, ret: " << std::endl;
    return -6;
  }
  defer(avcodec_close(audio_codec_ctx));

  if (!(frame = av_frame_alloc())) {
    std::cerr << "alloc frame failed" << std::endl;
    return -7;
  }
  defer(av_frame_free(&frame));

  if (!(packet = av_packet_alloc())) {
    std::cerr << "alloc packet failed" << std::endl;
    return -8;
  }
  defer(av_packet_free(&packet));

  if (!(swr_ctx = swr_alloc())) {
    std::cerr << "alloc audio swr ctx failed" << std::endl;
    return 0;
  }
  defer(swr_free(&swr_ctx));

  // const define
  int PCM_CHANNEL_LAYOUT = AV_CH_LAYOUT_STEREO;
  AVSampleFormat PCM_SAMPLE_FMT = AV_SAMPLE_FMT_S16;
  int PCM_SAMPLE_RATE = audio_codec_ctx->sample_rate;
  int AAC_SAMPLES_PER_FRAME = 1024;

  swr_alloc_set_opts(swr_ctx,
                     PCM_CHANNEL_LAYOUT,
                     PCM_SAMPLE_FMT,
                     PCM_SAMPLE_RATE,
                     audio_codec_ctx->channels,
                     audio_codec_ctx->sample_fmt,
                     audio_codec_ctx->sample_rate,
                     0,
                     nullptr);

  if ((ret = swr_init(swr_ctx)) < 0) {
    std::cerr << "init swr ctx failed, ret: " << ret << std::endl;
    return -9;
  }
  defer(swr_close(swr_ctx));

  bytes_per_frame =
      AAC_SAMPLES_PER_FRAME
          * av_get_bytes_per_sample(PCM_SAMPLE_FMT)
          * av_get_channel_layout_nb_channels(PCM_CHANNEL_LAYOUT);
  if (!(swr_out_buffer = (uint8_t *) av_malloc(bytes_per_frame))) {
    std::cerr << "malloc memory failed" << std::endl;
    return -10;
  }
  defer(av_free(swr_out_buffer));

  while (av_read_frame(video_ctx, packet) >= 0) {
    defer(av_packet_unref(packet));

    if (packet->stream_index != audio_stream_id) {
      continue;
    }

    if ((ret = avcodec_send_packet(audio_codec_ctx, packet)) < 0) {
      std::cerr << "send packet to codec ctx failed, ret: " << std::endl;
      return -9;
    }

    while (ret >= 0) {
      if ((ret = avcodec_receive_frame(audio_codec_ctx, frame)) < 0) {
        if (ret == AVERROR_EOF || ret == AVERROR(EAGAIN)) {
          continue;
        }

        std::cerr << "receive frame from audio codec ctx failed, ret: " << std::endl;
        return -10;
      }
      defer(av_frame_unref(frame));

      // handle
      int swr_out_samples = swr_convert(swr_ctx,
                                        &swr_out_buffer,
                                        AAC_SAMPLES_PER_FRAME,
                                        (const uint8_t **) frame->data,
                                        frame->nb_samples);
      bytes_per_frame = swr_out_samples
          * av_get_bytes_per_sample((AVSampleFormat) PCM_SAMPLE_FMT)
          * av_get_channel_layout_nb_channels(PCM_CHANNEL_LAYOUT);
      ofs.write((const char *) swr_out_buffer, bytes_per_frame);
    }
  }

  // flush
  if ((ret = avcodec_send_packet(audio_codec_ctx, nullptr)) < 0) {
    std::cerr << "send packet to audio codec ctx failed, ret: " << ret << std::endl;
    return -11;
  }

  while (ret >= 0) {
    if ((ret = avcodec_receive_frame(audio_codec_ctx, frame)) < 0) {
      if (ret == AVERROR_EOF || ret == AVERROR(EAGAIN)) {
        continue;
      }

      std::cerr << "receive frame from audio codec ctx failed, ret: " << std::endl;
      return -10;
    }
    defer(av_frame_unref(frame));

    // handle
    int swr_out_samples = swr_convert(swr_ctx,
                                      &swr_out_buffer,
                                      AAC_SAMPLES_PER_FRAME,
                                      (const uint8_t **) frame->data,
                                      frame->nb_samples);
    bytes_per_frame = swr_out_samples
        * av_get_bytes_per_sample((AVSampleFormat) frame->format)
        * frame->channels;
    ofs.write((const char *) swr_out_buffer, bytes_per_frame);
  }

  return 0;
}