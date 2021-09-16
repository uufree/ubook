#include <iostream>
#include <cstring>
#include "avio.h"
#include "defer.h"

extern "C" {
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
#include <libavutil/file.h>
}

/// demo 1
int AvIODemo::avio_malloc_free() {
  int buffer_size = 1024;
  unsigned char *buffer = new(std::nothrow) unsigned char[1024];
  if (buffer == nullptr) {
    std::cerr << "alloc buffer failed" << std::endl;
    return -1;
  }
  defer(delete[] buffer);

  AVIOContext *context = avio_alloc_context(buffer, buffer_size, 1, nullptr, nullptr, nullptr, nullptr);
  if (context == nullptr) {
    std::cerr << "alloc avio context failed" << std::endl;
    return -2;
  }
  avio_context_free(&context);
  context = nullptr;
  return 0;
}

/// demo2
int AvIODemo::read_and_write() {
  int buffer_size = 1024;
  uint8_t *buffer = (uint8_t*)av_malloc(buffer_size);
  if (buffer == nullptr) {
    std::cerr << "alloc buffer failed" << std::endl;
    return -1;
  }
  defer(av_free(buffer));

  AVIOContext *context = avio_alloc_context(buffer, buffer_size, 1, nullptr, nullptr, nullptr, nullptr);
  if (context == nullptr) {
    std::cerr << "alloc avio context failed" << std::endl;
    return -2;
  }

  /// 这块为啥行不通呢？
  avio_w8(context, 6);
  std::cout << avio_r8(context) << std::endl;
  std::cout << avio_size(context) << std::endl;

  avio_context_free(&context);
  context = nullptr;
  return 0;
}

/// demo3
struct buffer_data {
  uint8_t *buf;
  size_t buf_size;
};

int read_packet(void *opaque, uint8_t *buf, int buf_size) {
  buffer_data *bd = (buffer_data*)opaque;
  buf_size = buf_size >= bd->buf_size ? buf_size : bd->buf_size;
  if (!buf_size) {
    return AVERROR_EOF;
  }

  memcpy(buf, bd->buf, buf_size);
  bd->buf += buf_size;
  bd->buf_size -= buf_size;

  return buf_size;
}

int AvIODemo::read_from_memory(const char *video_path) {
  AVIOContext *avio_context;
  AVFormatContext *format_context;
  uint8_t *avio_ctx_buffer = nullptr;
  int avio_ctx_buffer_size = 4096;
  buffer_data bd = {nullptr, 0};
  int ret = 0;

  // 1. reaf video
  if ((ret = av_file_map(video_path, &bd.buf, &bd.buf_size, 0, nullptr)) < 0) {
    std::cerr << "call av_file_map failed, err: " << av_err2str(ret) << std::endl;
    return -1;
  }
  defer(av_file_unmap(bd.buf, bd.buf_size));

  // 2. alloc avio context buffer
  if (!(avio_ctx_buffer = (uint8_t*)av_malloc(avio_ctx_buffer_size))) {
    std::cerr << "alloc avio context buffer failed" << std::endl;
    return -2;
  }
  defer(av_free(avio_ctx_buffer));

  // 3. alloc avio context
  if (!(avio_context = avio_alloc_context(avio_ctx_buffer, avio_ctx_buffer_size, 0, &bd, read_packet, nullptr, nullptr))) {
    std::cerr << "alloc avio context failed" << std::endl;
    return -3;
  }
  defer(avio_context_free(&avio_context));

  // 4. alloc format context
  if (!(format_context = avformat_alloc_context())) {
    std::cerr << "alloc format context failed" << std::endl;
    return -4;
  }
  defer(avformat_free_context(format_context));

  // 5. fill pb
  format_context->pb = avio_context;

  // 6. open format context
  if ((ret = avformat_open_input(&format_context, nullptr, nullptr, nullptr)) < 0) {
    std::cerr << "open format context failed, err: " << av_err2str(ret) << std::endl;
    return -5;
  }
  defer(avformat_close_input(&format_context));

  return 0;
}

int AvIODemo::avio_read(const char *video_path) {
  AVFormatContext *format_context = nullptr;
  AVIOContext *avio_context = nullptr;
  uint8_t *buffer = nullptr;
  uint8_t *avio_ctx_buffer = nullptr;
  size_t buffer_size = 0;
  size_t avio_ctx_buffer_size = 4096;
  buffer_data bd = {nullptr, 0};
  int ret = 0;

  if ((ret = av_file_map(video_path, &buffer, &buffer_size, 0, nullptr)) < 0) {
    std::cerr << "call av_file_map failed, ret: " << ret << std::endl;
    return -1;
  }
  bd.buf = buffer;
  bd.buf_size = buffer_size;
  defer(av_file_unmap(buffer, buffer_size));

  if (!(format_context = avformat_alloc_context())) {
    std::cerr << "alloc format context failed" << std::endl;
    return -2;
  }
  defer(avformat_free_context(format_context));

  if (!(avio_ctx_buffer = (uint8_t *) av_malloc(avio_ctx_buffer_size))) {
    std::cerr << "alloc avio_ctx buffer failed" << std::endl;
    return -3;
  }

  if (!(avio_context =
            avio_alloc_context(avio_ctx_buffer, avio_ctx_buffer_size, 0, &bd, &read_packet, nullptr, nullptr))) {
    std::cerr << "alloc avio_ctx failed" << std::endl;
    return -4;
  }
  defer(avio_context_free(&avio_context));

  format_context->pb = avio_context;

  if ((ret = avformat_open_input(&format_context, nullptr, nullptr, nullptr)) < 0) {
    std::cerr << "open format failed" << std::endl;
    return -5;
  }
  defer(avformat_close_input(&format_context));

  if (0 > (ret = avformat_find_stream_info(format_context, nullptr))) {
    std::cerr << "call avformat_find_stream_info failed" << std::endl;
    return -6;
  }

  av_dump_format(format_context, 0, "", 0);

  return 0;
};