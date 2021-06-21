#include <string>
#include <atomic>

#include "st_audio_extract.h"
#include "defer.h"

extern "C" {
#include <libavutil/log.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
}

// 一次采样占用2个字节
const int WAVE_BITS_PER_SAMPLE = 16;
const int DEFAULT_AUDIO_OUTPUT_SAMPLE_RATE = 16000;
const int DEFAULT_AUDIO_OUTPUT_CHANNEL_LAYOUT = AV_CH_LAYOUT_MONO;
const AVSampleFormat DEFAULT_AUDIO_OUTPUT_SAMPLE_FORMAT = AV_SAMPLE_FMT_S16;
const int DEFAULT_AUDIO_INPUT_SAMPLES_PER_FRAME = 1024;

// wave = header + format + data description + row data
struct WaveHeader {
  char name[4];                       // 内容为""RIFF
  unsigned int block_size;            // 最后填写，WAVE格式音频的大小//unsigned long dwSize;
  char type[4];                       // 内容为"WAVE"
};

struct WaveFormat {
  char name[4];                       // 内容为"fmt "
  unsigned int block_size;            // 内容为WAVE_FMT占的字节数，为16
  unsigned short format_tag;          // 如果为PCM，改值为 1
  unsigned short channels;            // 通道数，单通道=1，双通道=2
  unsigned int sample_rate;           // 采样频率
  unsigned int avg_bytes_per_second;  // sample_rate * channels * bits_per_sample / 8
  unsigned short block_align;         // channels * bits_per_sample / 8
  unsigned short bits_per_sample;     // 每个采样点的bit数，8bits=8, 16bits=16
};

struct WaveData {
  char name[4];                       // 内容为"data"
  unsigned int data_size;             // sample_rate * channels * bits_per_sample / 8
};

/// @brief 音频提取实现类
/// @note 没有在library级别限制可以并发进行的音频提取数量，需要在业务层进行并发提取的限制
/// @note 使用之前，需要call Init()
class AudioExtract final {
 public:
  AudioExtract(const char *input_path, const char *audio_path) : input_path_(input_path), audio_path_(audio_path) {};
  AudioExtract(const AudioExtract &) = delete;
  AudioExtract(AudioExtract &&) = delete;
  AudioExtract &operator=(const AudioExtract &) = delete;
  AudioExtract &operator=(AudioExtract &&) = delete;
  ~AudioExtract() {}

  /// @brief 全局初始化函数，注册ffmpeg的解码器
  static void Init() {
    av_register_all();
  }

  /// @brief 音频提取 -> 音频转码(aac -> pcm -> wav) -> 音频重采样(xxx -> 16k,单声道)
  int Run(char *errmsg) {
    // ffmpeg err msg
    char ffmpeg_errstr[256];

    // 视频输入context
    AVFormatContext *input_ctx = NULL;
    int err_code = avformat_open_input(&input_ctx, input_path_, NULL, NULL);
    if (err_code < 0) {
      av_strerror(err_code, ffmpeg_errstr, sizeof(ffmpeg_errstr));
      sprintf(errmsg,
              "cannot open file: %s, ffmpeg errcode: %d, ffmpeg errmsg: %s, please check if already call Init() function!\n",
              input_path_,
              err_code,
              ffmpeg_errstr);
      return -1;
    }
    // 清理context资源
    defer(avformat_close_input(&input_ctx);avformat_free_context(input_ctx););

    // 找到质量最好的音频流
    int audio_stream_index = av_find_best_stream(input_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    if (audio_stream_index < 0) {
      sprintf(errmsg, "find best audio stream failed, please check input file: %s\n", input_path_);
      return -3;
    }
    // 音频流参数信息
    AVCodecParameters *input_audio_codecpar = input_ctx->streams[audio_stream_index]->codecpar;

    // input video data -> pcm
    // 构造 decoder
    AVCodec *input_audio_codec = avcodec_find_decoder(input_audio_codecpar->codec_id);
    AVCodecContext *input_audio_codec_context = avcodec_alloc_context3(NULL);
    if (input_audio_codec_context == NULL) {
      sprintf(errmsg, "create audio codec failed\n");
      return -4;
    }
    avcodec_parameters_to_context(input_audio_codec_context, input_audio_codecpar);
    err_code = avcodec_open2(input_audio_codec_context, input_audio_codec, NULL);
    if (err_code < 0) {
      av_strerror(err_code, ffmpeg_errstr, sizeof(ffmpeg_errstr));
      sprintf(errmsg, "open audio codec failed, ffmpeg errcode: %d, ffmpeg errmsg: %s\n", err_code, ffmpeg_errstr);
      return -5;
    }

    // 清理decoder 资源
    defer(avcodec_close(input_audio_codec_context);avcodec_free_context(&input_audio_codec_context););

    //frame->16bit 44100 PCM 统一音频采样格式与采样率
    //创建swrcontext上下文件
    SwrContext *swr_context = swr_alloc();
    // 输入音频的采样格式
    AVSampleFormat in_audio_format = input_audio_codec_context->sample_fmt;
    // 输出音频的采样格式
    AVSampleFormat out_audio_format = DEFAULT_AUDIO_OUTPUT_SAMPLE_FORMAT;
    // 输入音频的采样率
    int in_audio_sample_rate = input_audio_codec_context->sample_rate;
    // 输出音频的采样率
    int out_audio_sample_rate = DEFAULT_AUDIO_OUTPUT_SAMPLE_RATE;
    // 输入音频的声道布局
    uint64_t in_ch_layout = av_get_default_channel_layout(input_audio_codec_context->channels);
    // 输出音频的声道布局
    uint64_t out_ch_layout = DEFAULT_AUDIO_OUTPUT_CHANNEL_LAYOUT;

    //给Swrcontext 分配空间，设置公共参数
    swr_alloc_set_opts(swr_context, out_ch_layout, out_audio_format, out_audio_sample_rate,
                       in_ch_layout, in_audio_format, in_audio_sample_rate, 0, NULL);

    // 初始化
    err_code = swr_init(swr_context);
    if (err_code < 0) {
      av_strerror(err_code, ffmpeg_errstr, sizeof(ffmpeg_errstr));
      sprintf(errmsg, "init swr context failed, ffmpeg errcode: %d, ffmpeg errmsg: %s\n", err_code, ffmpeg_errstr);
      return -6;
    }
    // 清理swr资源
    defer(swr_close(swr_context);swr_free(&swr_context););

    // 获取输出声道数量
    int output_channels = av_get_channel_layout_nb_channels(DEFAULT_AUDIO_OUTPUT_CHANNEL_LAYOUT);

    // 获取输出文件的nb samples
    // 播放帧数 = 采样频率 / nb samples
    // dst nb smaples = src nb samples * dst sample rate / src sample rate
    int dst_nb_samples =
        av_rescale_rnd(DEFAULT_AUDIO_INPUT_SAMPLES_PER_FRAME,
                       DEFAULT_AUDIO_OUTPUT_SAMPLE_RATE,
                       in_audio_sample_rate,
                       AV_ROUND_UP);

    // 分配每一帧编码数据需要的最大空间
    // 最大假设为在1帧中，包含1s采样的所有数据。实际上，wav 1帧包含
    // wav output buffer = 1帧包含的nb_smaples * 一个采样位数 * channels / 8
    uint8_t *out_buffer =
        (uint8_t *) av_malloc(dst_nb_samples * output_channels * WAVE_BITS_PER_SAMPLE / 8);
    if (out_buffer == NULL) {
      sprintf(errmsg, "alloc output buffer resource failed\n");
      return -6;
    }
    defer (av_free(out_buffer));

    // audio -> packet
    AVPacket *packet = (AVPacket *) av_malloc(sizeof(AVPacket));
    if (packet == NULL) {
      sprintf(errmsg, "alloc packet resource failed\n");
      return -7;
    }
    defer(av_packet_unref(packet);av_packet_free(&packet));

    //解压缩后存放的数据帧的对象
    AVFrame *frame = av_frame_alloc();
    if (frame == NULL) {
      sprintf(errmsg, "alloc frame resource failed\n");
      return -8;
    }
    defer(av_frame_unref(frame);av_frame_free(&frame););

    //开始读取源文件，进行解码
    while (1) {
      // 跳过读取数据失败的帧
      err_code = av_read_frame(input_ctx, packet);
      if (err_code < 0) {
        av_strerror(err_code, ffmpeg_errstr, sizeof(ffmpeg_errstr));
        sprintf(errmsg, "read frame ret: %d, maybe error or EOF, errmsg: %s\n", err_code, ffmpeg_errstr);
        break;
      }
      defer(av_packet_unref(packet););

      // 跳过视频帧
      if (packet->stream_index != audio_stream_index) {
        continue;
      }

      // 将packet发送到解码器中
      avcodec_send_packet(input_audio_codec_context, packet);
      // 从解码器中解出存有封装格式的数据（aac,mp3等）
      err_code = avcodec_receive_frame(input_audio_codec_context, frame);
      if (err_code != 0) {
        av_strerror(err_code, ffmpeg_errstr, sizeof(ffmpeg_errstr));
        printf("decode audio frame failed, ffmpeg errcode: %d, ffmpeg errmsg: %s\n", err_code, ffmpeg_errstr);
        continue;
      }
      defer(av_frame_unref(frame););

      // aac: nb_samples = 1024, bytes_per_sample = 4 (32位)
      int bytes_per_frame = frame->nb_samples * av_get_bytes_per_sample((AVSampleFormat) frame->format);
      // 返回输出编码输出格式（wav）信息：每帧包含out_samples个采样信息
      int out_samples =
          swr_convert(swr_context, &out_buffer, frame->nb_samples, (const uint8_t **) frame->data, frame->nb_samples);
      if (out_samples < 0) {
        av_strerror(out_samples, ffmpeg_errstr, sizeof(ffmpeg_errstr));
        printf("convert audio frame failed, ffmpeg errcode: %d, ffmpeg errmsg: %s\n", out_samples, ffmpeg_errstr);
        continue;
      }

      bytes_per_frame = av_samples_get_buffer_size(NULL,
                                                   output_channels,
                                                   out_samples,
                                                   DEFAULT_AUDIO_OUTPUT_SAMPLE_FORMAT,
                                                   1);
      // 写入缓存
      data_ += std::string((char *) out_buffer, bytes_per_frame);
    }

    if (0 == data_.size()) {
      sprintf(errmsg, "extract audio failed, pcm data size is zero\n");
      return -9;
    }

    // 向wav文件中写入数据
    FILE *wav_file = fopen(audio_path_, "wb+");
    if (wav_file == NULL) {
      sprintf(errmsg, "open and create audio file: %s failed\n", audio_path_);
      return -10;
    }
    defer(fclose(wav_file));

    WriteWaveHeader(wav_file);
    WriteWaveFormat(wav_file, output_channels, out_audio_sample_rate, WAVE_BITS_PER_SAMPLE);
    WriteWaveData(wav_file);

    return 0;
  }

 private:
  /// @brief 向wave file中写header
  int WriteWaveHeader(FILE *wav_file) {
    WaveHeader wave_header;
    memcpy(wave_header.name, "RIFF", strlen("RIFF"));
    memcpy(wave_header.type, "WAVE", strlen("WAVE"));
    wave_header.block_size = 36 + data_.size(); // header + format + data block，以每次采样的数据大小为单位进行计数（16 bits）

    rewind(wav_file);
    fwrite(&wave_header, sizeof(WaveHeader), 1, wav_file);
    return 0;
  }

  /// @brief 向wave file中写format
  int WriteWaveFormat(FILE *wav_file, int channels, int sample_rate, int bits) {
    WaveFormat wave_format;
    memcpy(wave_format.name, "fmt ", strlen("fmt "));
    wave_format.block_size = 16;
    wave_format.format_tag = 1;
    wave_format.channels = channels;
    wave_format.sample_rate = sample_rate;
    wave_format.bits_per_sample = bits;
    wave_format.avg_bytes_per_second =
        wave_format.sample_rate * wave_format.channels * wave_format.bits_per_sample / 8;
    wave_format.block_align = wave_format.channels * wave_format.bits_per_sample / 8;

    fseek(wav_file, sizeof(WaveHeader), SEEK_SET);
    fwrite(&wave_format, sizeof(WaveFormat), 1, wav_file);
    return 0;
  }

  /// @brief 向wave file中写data
  int WriteWaveData(FILE *wav_file) {
    WaveData wave_data;
    memcpy(wave_data.name, "data", strlen("data"));
    wave_data.data_size = data_.size();

    fseek(wav_file, sizeof(WaveHeader) + sizeof(WaveFormat), SEEK_SET);
    fwrite(&wave_data, sizeof(WaveData), 1, wav_file);

    fseek(wav_file, sizeof(WaveHeader) + sizeof(WaveFormat) + sizeof(WaveData), SEEK_SET);
    fwrite(data_.data(), sizeof(uint8_t), data_.size(), wav_file);

    return 0;
  }

 private:
  const char *input_path_;
  const char *audio_path_;

  std::string data_; // 缓存所有的pcm数据
};

void st_audio_extract_init() {
  AudioExtract::Init();
}

int st_audio_extract(const char *video_path, const char *audio_path, char *errmsg) {
  AudioExtract audio_extract(video_path, audio_path);
  return audio_extract.Run(errmsg);
}

int st_audio_convert(const char* raw_audio_path, const char* audio_path, char* errmsg) {
  AudioExtract audio_extract(raw_audio_path, audio_path);
  return audio_extract.Run(errmsg);
}
