# FFmpeg 4.4 Core APIs

[TOC]

## AvFormat

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

/// @brief Seek to timestamp ts.
/// @param [in] media file format ctx
/// @param [in] stream index
/// @param [in] smallest acceptable timestamp
/// @param [in] target timestamp
/// @param [in] largest acceptable timestamp
/// @param [in] 0 for default
/// @return >=0 on success, error code otherwise
int avformat_seek_file(AVFormatContext *s, int stream_index, int64_t min_ts, int64_t ts, int64_t max_ts, int flags);

/// @brief 丢弃所有内部缓冲的数据。 这在处理字节流中的不连续时很有用。适用于可以重新同步的格式。包括像 MPEG-TS/TS 这样的无头格式，但也应该与 NUT、Ogg 和有限的 AVI 等一起使用。
/// @param [in] media file format ctx
/// @return >=0 on success, error code otherwise
/// @note If you want a complete reset, it's better to open a new AVFormatContext.
/// @note This does not flush the AVIOContext (s->pb). If necessary, call avio_flush(s->pb) before calling this function.
int avformat_flush(AVFormatContext *s);

/// @brief 准备开始读取网络流
int av_read_play(AVFormatContext *s);

/// @beirf 暂停读取网络流
int av_read_pause(AVFormatContext *s);

/// @brief Allocate the stream private data and write the stream header to an output media file.
/// @param [in] media file handle
/// @param [in] An AVDictionary filled with AVFormatContext and muxer-private options.
/// @return >=0 on success, error code otherwise
int avformat_write_header(AVFormatContext *s, AVDictionary **options);

/// @brief Write the stream trailer to an output media file
/// @param [in] media file handler
/// @return 0 if OK, AVERROR_xxx on error
/// @note May only be called after a successful call to avformat_write_header
int av_write_trailer(AVFormatContext *s);

/// @brief Write a packet to an output media file. 
/// @param [in] media file handler
/// @param [in] data pkg. pkg->stream_index | pkg.pts | pkg.dts | pkg.timebase must be set; 
/// @return < 0 on error, = 0 if OK, 1 if flushed and there is no more data to flush
/// @note 此函数将数据包直接传递给muxer，无需任何缓冲或重新排序。调用者需要正确排步数据包。如果希望libavformat处理数据包交错，应该调用 av_interleaved_write_frame() 而不是这个功能。
/// @note This function passes the packet directly to the muxer, without any buffering or reordering.
int av_write_frame(AVFormatContext *s, AVPacket *pkt);

/// @brief Write a packet to an output media file ensuring correct interleaving.
/// @param [in] media file handler
/// @param [in] data pkg. pkg->stream_index | pkg.pts | pkg.dts | pkg.timebase must be set; 
/// @return 0 on success, a negative AVERROR on error
/// @note This function will buffer the packets internally as needed to make sure the packets in the output file are properly interleaved in the order of increasing dts.
int av_interleaved_write_frame(AVFormatContext *s, AVPacket *pkt);

/// @brief Get timing information for the data currently output.
/// @param [in] media file handler
/// @param [in] stream index
/// @param [out] DTS of the last packet output for the stream
/// @param [out] absolute time when that packet whas output, in microsecond
/// @return 0 if OK
int av_get_output_timestamp(struct AVFormatContext *s, int stream,
                            int64_t *dts, int64_t *wall);

/// @brief Send a nice dump of a packet to the specified file stream.
/// @param [in] file handler
/// @param [in] packet to dump
/// @param [in] True if the payload must be displayed, too.
/// @param [in] AVStream that the packet belongs to
void av_pkt_dump2(FILE *f, const AVPacket *pkt, int dump_payload, const AVStream *st);

/// @brief Print detailed information about the input or output format
/// @param [in] media file handler
/// @param [in] index of the stream to dump information about
/// @param [in] the URL to print, such as source or destination file
/// @param [in] Select whether the specified context is an input(0) or output(1)
void av_dump_format(AVFormatContext *ic, int index, const char *url, int is_output);

/// @brief Get the internal codec timebase from a stream.
/// @return rational
AVRational av_stream_get_codec_timebase(const AVStream *st);

/// @brief Transfer internal timing information from one stream to another
/// @param [in] target output format for ost
/// @param [in] output stream which needs timings copy and adjustments
/// @param [in] reference input stream to copy timings from
/// @param [in] define from where the stream codec timebase needs to be imported
/// @return 0 on success, a negative AVERROR on error
/// @note This function is useful when doing stream copy.
enum AVTimebaseSource {
    AVFMT_TBCF_AUTO = -1,
    AVFMT_TBCF_DECODER,
    AVFMT_TBCF_DEMUXER,
#if FF_API_R_FRAME_RATE
    AVFMT_TBCF_R_FRAMERATE,
#endif
};
int avformat_transfer_internal_stream_timing_info(const AVOutputFormat *ofmt,
                                                  AVStream *ost, const AVStream *ist,
                                                  enum AVTimebaseSource copy_tb);
```

## AvCodec

### avcodec

```c++
/// @brief avcodec utils
unsigned avcodec_version(void);
const char *avcodec_configuration(void);
const char *avcodec_license(void);

/// @brief Allocate/Free an AVCodecContext and set its fields to default values
/// @param [in] codec
/// @return An AVCodecContext filled with default values or NULL on failure.
AVCodecContext *avcodec_alloc_context3(const AVCodec *codec);
void avcodec_free_context(AVCodecContext **avctx);

/// @brief Fill the parameters struct based on the values from the supplied codec context.
/// @return >= 0 on success, a negative AVERROR code on failure
int avcodec_parameters_from_context(AVCodecParameters *par, const AVCodecContext *codec);

/// @brief Fill the codec context based on the values from the supplied codec parameters.
/// @return >= 0 on success, a negative AVERROR code on failure.
int avcodec_parameters_to_context(AVCodecContext *codec, const AVCodecParameters *par);

/// @brief Initialize the AVCodecContext to use the given AVCodec. 
/// @return zero on success, a negative value on error
int avcodec_open2(AVCodecContext *avctx, const AVCodec *codec, AVDictionary **options);
int avcodec_close(AVCodecContext *avctx);

/// @brief Decode a subtitle message.
/// @param [in] codec ctx
/// @param [out] The preallocated AVSubtitle in which the decoded subtitle will be stored,  must be freed with avsubtitle_free if *got_sub_ptr is set.
/// @param [in, out] zero if no subtitle could be decompressed, otherwise, it is nonzero.
/// @param [in] the input AVPacket containing the input buffer.
/// @return Return a negative value on error, otherwise return the number of bytes used.
int avcodec_decode_subtitle2(AVCodecContext *avctx, AVSubtitle *sub, int *got_sub_ptr, AVPacket *avpkt);
int avcodec_encode_subtitle(AVCodecContext *avctx, uint8_t *buf, int buf_size, const AVSubtitle *sub);
void avsubtitle_free(AVSubtitle *sub);

/// @brief send pkg to decoder
/// @param [in] avctx codec context
/// @param [in] this will be a single video frame, or several complete audio frames.
/// @return 0 on success, otherwise negative error code:
/// AVERROR(EAGAIN): input is not accepted in the current state, user must read output with avcodec_receive_frame() 
/// AVERROR_EOF: the decoder has been flushed, and no new packets can be sent to it
/// AVERROR(EINVAL): codec not opened, it is an encoder, or requires flush
/// AVERROR(ENOMEM): failed to add packet to internal queue
int avcodec_send_packet(AVCodecContext *avctx, const AVPacket *avpkt);

/// @brief recv frame from decoder
/// @param [in] avctx codec context
/// @param [out] the function will always call av_frame_unref(frame) before doing anything else.
/// @return 0 on success, otherwise negative error code:
/// AVERROR(EAGAIN): output is not available in this state, user must try to send new input
/// AVERROR_EOF: the decoder has been fully flushed
/// AVERROR(EINVAL): codec not opened, or it is an encoder
int avcodec_receive_frame(AVCodecContext *avctx, AVFrame *frame);

/// @brief send frame to encoder
/// @param [in] codec ctx
/// @param [in] video frame 
/// @return 0 on success, otherwise negative error code:
/// AVERROR(EAGAIN): input is not accepted in the current state, user must read output with avcodec_receive_packet()
/// AVERROR_EOF: the encoder has been flushed
/// AVERROR(EINVAL): codec not opened, refcounted_frames not set, it is a decoder, or requires flush
/// @note If AV_CODEC_CAP_VARIABLE_FRAME_SIZE is set, then each frame can have any number of samples.
/// @note If it is not set, frame->nb_samples must be equal to avctx->frame_size for all frames except the last.
int avcodec_send_frame(AVCodecContext *avctx, const AVFrame *frame);

/// @brief read pkg from encoder
/// @param [in] codec context
/// @param [out] pkg
/// @return 0 on success, otherwise negative error code:
/// AVERROR(EAGAIN): output is not available in the current state, user must try to send input
/// AVERROR_EOF: the encoder has been fully flushed, and there will be no more output packets
/// AVERROR(EINVAL): codec not opened, or it is a decoder
int avcodec_receive_packet(AVCodecContext *avctx, AVPacket *avpkt);

/// @brief 重要，但是不会用。在遇到硬件解码问题时，需要额外关注这个问题
int avcodec_get_hw_frames_parameters(AVCodecContext *avctx,
                                     AVBufferRef *device_ref,
                                     enum AVPixelFormat hw_pix_fmt,
                                     AVBufferRef **out_frames_ref);

/// @brief Fill AVFrame audio data and linesize pointers.
/// @param [in] frame->nb_samples must be set prior to calling the function. This function fills in frame->data,
/// frame->extended_data, frame->linesize[0].
/// @param [in] channel count
/// @param [in] sample format
/// @param [in] 将这个buf中的数据填充至AVFrame中
/// @param [in] buf size
/// @param [in] align, default = 0
/// @return >=0 on success, negative error code on failure
int avcodec_fill_audio_frame(AVFrame *frame, int nb_channels,
                             enum AVSampleFormat sample_fmt, const uint8_t *buf,
                             int buf_size, int align);

/// @brief Reset the internal codec state / flush internal buffers. Should be called when seeking or when switching to a different stream.
void avcodec_flush_buffers(AVCodecContext *avctx);

/// @brief Return codec bits per sample.
int av_get_bits_per_sample(enum AVCodecID codec_id);

/// @brief Return audio frame duration.
/// @param [in] avcodec ctx
/// @param [in] size of the frame, or 0 if unknown
/// @return frame duration
int av_get_audio_frame_duration(AVCodecContext *avctx, int frame_bytes);
```

### bsf

```c++
/// @brief get a bsf filter
const AVBitStreamFilter *av_bsf_get_by_name(const char *name);

/// @brief Iterate over all registered bitstream filters.
const AVBitStreamFilter *av_bsf_iterate(void **opaque);

/// @brief Allocate/Free a context for a given bitstream filter.
int av_bsf_alloc(const AVBitStreamFilter *filter, AVBSFContext **ctx);
void av_bsf_free(AVBSFContext **ctx);

/// @brief Prepare the filter for use, after all the parameters and options have been set
int av_bsf_init(AVBSFContext *ctx);

/// @brief Submit a packet for filtering.
int av_bsf_send_packet(AVBSFContext *ctx, AVPacket *pkt);

/// @brief Retrieve a filtered packet.
int av_bsf_receive_packet(AVBSFContext *ctx, AVPacket *pkt);

/// @brief Reset the internal bitstream filter state
/// @note Should be called e.g. when seeking.
void av_bsf_flush(AVBSFContext *ctx);

/// @brief Allocate/Free empty list of bitstream filters.
AVBSFList *av_bsf_list_alloc(void);
void av_bsf_list_free(AVBSFList **lst);

/// @brief Append bitstream filter to the list of bitstream filters.
int av_bsf_list_append(AVBSFList *lst, AVBSFContext *bsf);

/// @brief Finalize list of bitstream filters.
/// @note This function will transform @ref AVBSFList to single @ref AVBSFContext, so the whole chain of bitstream filters can be treated as single filter
/// @note If the call is successful, @ref AVBSFList structure is freed and lst will be set to NULL.
int av_bsf_list_finalize(AVBSFList **lst, AVBSFContext **bsf);

/// @brief Parse string describing list of bitstream filters and create single @ref AVBSFContext describing the whole chain of bitstream filters.
int av_bsf_list_parse_str(const char *str, AVBSFContext **bsf);
```

### codec

```c++
/// @brief Iterate over all registered codecs.
const AVCodec *av_codec_iterate(void **opaque);

/// @brief Find a registered decoder/encoder with a matching codec ID.
AVCodec *avcodec_find_decoder(enum AVCodecID id);
AVCodec *avcodec_find_encoder(enum AVCodecID id);

/// @brief Find a registered decoder/encoder with the specified name.
AVCodec *avcodec_find_decoder_by_name(const char *name);
AVCodec *avcodec_find_encoder_by_name(const char *name);

/// @return a non-zero number if codec is a decoder/encoder, zero otherwise
int av_codec_is_encoder(const AVCodec *codec);
int av_codec_is_decoder(const AVCodec *codec);

/// @brief Retrieve supported hardware configurations for a codec.
const AVCodecHWConfig *avcodec_get_hw_config(const AVCodec *codec, int index);

/// @brief Get the type of the given codec.
enum AVMediaType avcodec_get_type(enum AVCodecID codec_id);

/// @brief Get the name of a codec.
const char *avcodec_get_name(enum AVCodecID id);

/// @brief Allocate a new AVCodecParameters and set its fields to default values
AVCodecParameters *avcodec_parameters_alloc(void);

/// @brief Free an AVCodecParameters instance and everything associated with it and write NULL to the supplied pointer.
void avcodec_parameters_free(AVCodecParameters **par);

/// @brief Copy the contents of src to dst.
/// @return >= 0 on success, a negative AVERROR code on failure.
int avcodec_parameters_copy(AVCodecParameters *dst, const AVCodecParameters *src);
```

### packet

```c++
/// @brief Allocate an AVPacket and set its fields to default values.
/// @note this only allocates the AVPacket itself, not the data buffers. Those must be allocated through other means such as av_new_packet.
AVPacket *av_packet_alloc(void);
void av_packet_free(AVPacket **pkt);

/// @brief Create a new packet that references the same data as src.
AVPacket *av_packet_clone(const AVPacket *src);

/// @brief Allocate the payload of a packet and initialize its fields with default values.
int av_new_packet(AVPacket *pkt, int size);

/// @brief Reduce packet size, correctly zeroing padding
void av_shrink_packet(AVPacket *pkt, int size);

/// @brief Increase packet size, correctly zeroing padding
int av_grow_packet(AVPacket *pkt, int grow_by);

/// @brief Initialize a reference-counted packet from av_malloc()ed data.
/// @param [in] This function will set the data, size, and buf fields, all others are left untouched.
/// @param [in] If this function returns successfully, the data is owned by the underlying AVBuffer. The caller may not access the data through other means.
/// @param [in] size of data in bytes, without the padding
/// @return 0 on success, a negative AVERROR on error
int av_packet_from_data(AVPacket *pkt, uint8_t *data, int size);

/// @brief Wrap an existing array as a packet side data.
int av_packet_add_side_data(AVPacket *pkt, enum AVPacketSideDataType type, uint8_t *data, size_t size);
void av_packet_free_side_data(AVPacket *pkt);

/// @brief setup a new reference to the data described by a given packet
int av_packet_ref(AVPacket *dst, const AVPacket *src);
void av_packet_unref(AVPacket *pkt);

/// @brief Move every field in src to dst and reset src.
void av_packet_move_ref(AVPacket *dst, AVPacket *src);

/// @brief Copy only "properties" fields from src to dst.
int av_packet_copy_props(AVPacket *dst, const AVPacket *src);

/// @brief Create a writable reference for the data described by a given packet, avoiding data copy if possible.
int av_packet_make_writable(AVPacket *pkt);

/// @brief Convert valid timing fields (timestamps / durations) in a packet from one timebase to another.
void av_packet_rescale_ts(AVPacket *pkt, AVRational tb_src, AVRational tb_dst);
```

## AvFilter

