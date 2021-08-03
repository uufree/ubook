# FFmpeg 4.4 Core APIs

[TOC]

## AVFormat

### avio

```c++
/// @brief Allocate and initialize an AVIOContext for buffered I/O. 
/// @param [in] 预先分配好的一块buffer
/// @param [in] buffer size
/// @param [in] 如果这块buffer需要被写入，那就将flag置1；否则置0
/// @param [in] 用户自定义结构
/// @param [in] user read function
/// @param [in] user write function
/// @param [in] user seek function
/// @return  AVIOContext Pointer
AVIOContext *avio_alloc_context(
                  unsigned char *buffer,
                  int buffer_size,
                  int write_flag,
                  void *opaque,
                  int (*read_packet)(void *opaque, uint8_t *buf, int buf_size),
                  int (*write_packet)(void *opaque, uint8_t *buf, int buf_size),
                  int64_t (*seek)(void *opaque, int64_t offset, int whence));

/// @brief 释放AVIOContext Pointer
/// @param [in] alloc_*分配的pointer
/// @return no return
/// @note 二级指针使用完成后记得=nullptr，否则会产生非定义的crash
void avio_context_free(AVIOContext **s);

/// @brief AVIOContext Write Options
void avio_w8(AVIOContext *s, int b);
void avio_write(AVIOContext *s, const unsigned char *buf, int size);
void avio_wl64(AVIOContext *s, uint64_t val);
void avio_wb64(AVIOContext *s, uint64_t val);
void avio_wl32(AVIOContext *s, unsigned int val);
void avio_wb32(AVIOContext *s, unsigned int val);
void avio_wl24(AVIOContext *s, unsigned int val);
void avio_wb24(AVIOContext *s, unsigned int val);
void avio_wl16(AVIOContext *s, unsigned int val);
void avio_wb16(AVIOContext *s, unsigned int val);
int avio_put_str(AVIOContext *s, const char *str); /// @return number of bytes written

/// @brief AVIOContext Read Options
int          avio_r8  (AVIOContext *s);
unsigned int avio_rl16(AVIOContext *s);
unsigned int avio_rl24(AVIOContext *s);
unsigned int avio_rl32(AVIOContext *s);
uint64_t     avio_rl64(AVIOContext *s);
unsigned int avio_rb16(AVIOContext *s);
unsigned int avio_rb24(AVIOContext *s);
unsigned int avio_rb32(AVIOContext *s);
uint64_t     avio_rb64(AVIOContext *s);
int avio_get_str(AVIOContext *pb, int maxlen, char *buf, int buflen);	/// @return number of bytes read

/// @brief Other Options
/// @note whence可选：SEEK_CUR, SEEK_SET（将offset移动到某个位置）, SEEK_END
int64_t avio_seek(AVIOContext *s, int64_t offset, int whence);
int64_t avio_skip(AVIOContext *s, int64_t offset);
int64_t avio_size(AVIOContext *s);
/// @note 当且仅当在已经到达文件末尾或者发生读取错误时，返回非0值
int avio_feof(AVIOContext *s);
int avio_printf(AVIOContext *s, const char *fmt, ...);
/// @note 对于write stream，将buffer中的数据强制写入stream中；对于read stream来说，丢弃buffer中所有的数据
void avio_flush(AVIOContext *s);
```

### avfmt

```c++
/// @brief avfmt utils
unsigned avformat_version(void);
const char *avformat_configuration(void);
const char *avformat_license(void);

/// @brief Core APIS
int avformat_network_init(void);
int avformat_network_deinit(void);

/// @brief 遍历所有已经注册的muxers/demuxers
/// @param [in] a pointer where libavformat will store the iteration state. Must point to NULL to start the iteration.
/// @return the next registered muxer or NULL when the iteration is finished
const AVOutputFormat *av_muxer_iterate(void **opaque);
const AVInputFormat *av_demuxer_iterate(void **opaque);

/// @brief alloc/free format context
AVFormatContext *avformat_alloc_context(void);
void avformat_free_context(AVFormatContext *s);

/// @brief 在format中新增一个stream
AVStream *avformat_new_stream(AVFormatContext *s, const AVCodec *c);

/// @brief stream的side data操作
/// @note 具体类型详见AVPacketSideDataType
int av_stream_add_side_data(AVStream *st, enum AVPacketSideDataType type, uint8_t *data, size_t size);
uint8_t *av_stream_new_side_data(AVStream *stream, enum AVPacketSideDataType type, int size);
uint8_t *av_stream_get_side_data(const AVStream *stream, enum AVPacketSideDataType type, int *size);

/// @brief 根据output format分配一个AVFormatContext
/// @param [out] 待分配的avformat ctx
/// @param [in] output format。可以根据：av_muxer_iterate获取AVOuptutFormat
/// @param [in] format name
/// @param [in] filename, maybe null
/// @return >= 0 in case of success, a negative AVERROR code in case of failure
/// @note 在创建一个新的音视频文件时，可能需要使用这个函数
int avformat_alloc_output_context2(AVFormatContext **ctx, ff_const59 AVOutputFormat *oformat, const char *format_name, const char *filename);

/// @brief Find AVInputFormat based on the short name of the input format.
/// @note 这个函数可能不会用到
ff_const59 AVInputFormat *av_find_input_format(const char *short_name);

/// @brief Open an input stream and read the header. The codecs are not opened.
/// @param [in] fmt ctx
/// @param [in] url
/// @param [in] 无特殊情况，都指定为null。由ffmpeg根据header中的信息自行判断AVInputFormat
/// @param [in] options
/// @return 0 on success, a negative AVERROR on failure.
/// @note If you want to use custom IO, preallocate the format context and set its pb field.
int avformat_open_input(AVFormatContext **ps, const char *url, ff_const59 AVInputFormat *fmt, AVDictionary **options);
void avformat_close_input(AVFormatContext **s);

/// @brief Read packets of a media file to get stream information. 
/// This is useful for file formats with no headers such as MPEG.
/// This function also computes the real framerate in case of MPEG-2 repeat frame mode.
/// @param [in] fmt ctx
/// @param [in] options，默认情况下置空就行
/// @return >=0 if OK, AVERROR_xxx on error
int avformat_find_stream_info(AVFormatContext *ic, AVDictionary **options);

/// @brief 从format中寻找一个某种类型最佳的stream
/// @param [in] fmt ctx
/// @param [in] media type. 详见：AVMediaType
/// @param [in] -1 for automatic selection
/// @param [in] -1 for default
/// @param [in] if non-NULL, returns the decoder for the selected stream
/// @param [in] 0 for default
/// @return >= 0 in case of success, a negative AVERROR code in case of failure
int av_find_best_stream(AVFormatContext *ic,
                        enum AVMediaType type,
                        int wanted_stream_nb,
                        int related_stream,
                        AVCodec **decoder_ret,
                        int flags);

/// @brief 从streamm中读取一个可以成功解码的pkt
/// @param [in] fmt ctx
/// @param [out] packet
/// @return 0 if OK, < 0 on error or end of file. On error, pkt will be blank
/// @note: the returned packet is reference-counted
/// @note: The packet must be freed with av_packet_unref() when it is no longer needed.
/// @note: For video, the packet contains exactly one frame.
/// @note: For audio, it contains an integer number of frames if each frame has a known fixed size.
/// @note: pkt->pts, pkt->dts and pkt->duration are always set
/// @note: pkt->pts can be AV_NOPTS_VALUE if the video format has B-frames
int av_read_frame(AVFormatContext *s, AVPacket *pkt);

/*seek flags*/
#define AVSEEK_FLAG_BACKWARD 1 ///< seek backward
#define AVSEEK_FLAG_BYTE     2 ///< seeking based on position in bytes
#define AVSEEK_FLAG_ANY      4 ///< seek to any frame, even non-keyframes
#define AVSEEK_FLAG_FRAME    8 ///< seeking based on frame number

/// @brief Seek to the keyframe at timestamp.
/// @param [in] fmt ctx
/// @param [in] stream index
/// @param [in] timestamp
/// @param [in] 0 for default
/// @return >= 0 on success
int av_seek_frame(AVFormatContext *s, int stream_index, int64_t timestamp, int flags);

/// 
int avformat_seek_file(AVFormatContext *s, int stream_index, int64_t min_ts, int64_t ts, int64_t max_ts, int flags);
```



## AVCodec

## AVFilter

