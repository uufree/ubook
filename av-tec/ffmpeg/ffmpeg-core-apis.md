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

```c++
/// @brief utils
unsigned avfilter_version(void);
const char *avfilter_configuration(void);
const char *avfilter_license(void);

/// @brief Get the number of elements in a NULL-terminated array of AVFilterPads
int avfilter_pad_count(const AVFilterPad *pads);

/// @brief Get the name of an AVFilterPad.
const char *avfilter_pad_get_name(const AVFilterPad *pads, int pad_idx);

/// @brief Get the type of an AVFilterPad.
enum AVMediaType avfilter_pad_get_type(const AVFilterPad *pads, int pad_idx);

/// @brief Link two filters together.
/// @param [in] the source filter
/// @param [in] index of the output pad on the source filter
/// @param [in] index of the input pad on the destination filter
/// @param [in] index of the input pad on the destination filter
/// @return zero on success
int avfilter_link(AVFilterContext *src, unsigned srcpad, AVFilterContext *dst, unsigned dstpad);

/// @brief Free the link in *link, and set its pointer to NULL.
void avfilter_link_free(AVFilterLink **link);

/// @brief Make the filter instance process a command.
int avfilter_process_command(AVFilterContext *filter, const char *cmd, const char *arg, char *res, int res_len, int flags);

/// @brief Iterate over all registered filters.
const AVFilter *av_filter_iterate(void **opaque);

/// @brief Get a filter definition matching the given name.
const AVFilter *avfilter_get_by_name(const char *name);

/// @brief Initialize a filter with the supplied dictionary of options.
/// @param [in] ctx
/// @param [in] An AVDictionary filled with options for this filter.
/// @return 0 on success, a negative AVERROR on failure
int avfilter_init_dict(AVFilterContext *ctx, AVDictionary **options);

/// @brief Free a filter context. This will also remove the filter from its filtergraph's list of filters.
void avfilter_free(AVFilterContext *filter);

/// @brief Insert a filter in the middle of an existing link
/// @param [in] the link into which the filter should be inserted
/// @param [in] the filter to be inserted
/// @param [in] the input pad on the filter to connect
/// @param [in] the output pad on the filter to connect
/// @return zero on success
int avfilter_insert_filter(AVFilterLink *link, AVFilterContext *filt,
                           unsigned filt_srcpad_idx, unsigned filt_dstpad_idx);

/// @brief Allocate/Free a filter graph.
AVFilterGraph *avfilter_graph_alloc(void);
void avfilter_graph_free(AVFilterGraph **graph);

/// @brief Create a new filter instance in a filter graph.
/// @param [in] raph in which the new filter will be used
/// @param [in] the filter to create an instance of
/// @param [in] name to give to the new instance
/// @return the context of the newly created filter instance
AVFilterContext *avfilter_graph_alloc_filter(AVFilterGraph *graph,
                                             const AVFilter *filter,
                                             const char *name);

/// @brief Get a filter instance identified by instance name from graph.
AVFilterContext *avfilter_graph_get_filter(AVFilterGraph *graph, const char *name);

/// @brief Create and add a filter instance into an existing graph.
/// @param [out] In case of success put in *filt_ctx the pointer to the created filter instance, otherwise set *filt_ctx to NULL.
/// @param [in] filter
/// @param [in] name
/// @param [in] init args
/// @param [in] 未使用
/// @param [in] the filter graph
/// @return a negative AVERROR error code in case of failure,
int avfilter_graph_create_filter(AVFilterContext **filt_ctx, const AVFilter *filt,
                                 const char *name, const char *args, void *opaque,
                                 AVFilterGraph *graph_ctx);

/// @brief Enable or disable automatic format conversion inside the graph.
/// @note format conversion can still happen inside explicitly inserted scale and aresample filters.
enum {
    AVFILTER_AUTO_CONVERT_ALL  =  0, /**< all automatic conversions enabled */
    AVFILTER_AUTO_CONVERT_NONE = -1, /**< all automatic conversions disabled */
};
void avfilter_graph_set_auto_convert(AVFilterGraph *graph, unsigned flags);

/// @brief Check validity and configure all the links and formats in the graph.
int avfilter_graph_config(AVFilterGraph *graphctx, void *log_ctx);

/// @brief Allocate/Free a single AVFilterInOut entry
AVFilterInOut *avfilter_inout_alloc(void);
void avfilter_inout_free(AVFilterInOut **inout);

/*
* The buffersink and abuffersink filters are there to connect filter graphs to applications. They have a single   * input, connected to the graph, and no output. Frames must be extracted using av_buffersink_get_frame() or 	  *	 av_buffersink_get_samples().
*/

/// @brief Get a frame with filtered data from sink and put it in frame.
/// @param [in] buffersink filter context
/// @param [in] an allocated frame that will be filled with data. 
/// @param [in] a combination of AV_BUFFERSINK_FLAG_* flags
/// @return >= 0 in for success, a negative AVERROR code for failure.
int av_buffersink_get_frame_flags(AVFilterContext *ctx, AVFrame *frame, int flags);

/// @brief Set the frame size for an audio buffer sink.
void av_buffersink_set_frame_size(AVFilterContext *ctx, unsigned frame_size);

/// @brief Get a frame with filtered data from sink and put it in frame
/// @return >= 0 if a frame was successfully returned.
/// @return AVERROR(EAGAIN) if no frames are available at this point
/// @return AVERROR_EOF if there will be no more output frames on this sink
/// @return A different negative AVERROR code in other failure cases
int av_buffersink_get_frame(AVFilterContext *ctx, AVFrame *frame);

/// @brief Get a frame with filtered data from sink and put it in frame (audio)
int av_buffersink_get_samples(AVFilterContext *ctx, AVFrame *frame, int nb_samples);

/// @brief Allocate a new AVBufferSrcParameters instance
AVBufferSrcParameters *av_buffersrc_parameters_alloc(void);

/// @brief Initialize the buffersrc or abuffersrc filter with the provided parameters
int av_buffersrc_parameters_set(AVFilterContext *ctx, AVBufferSrcParameters *param);

/// @brief Add a frame to the buffer source
int av_buffersrc_write_frame(AVFilterContext *ctx, const AVFrame *frame);

/// @brief Add a frame to the buffer source.
int av_buffersrc_add_frame_flags(AVFilterContext *buffer_src,
                                 AVFrame *frame, int flags);

/// @brief Close the buffer source after EOF.
int av_buffersrc_close(AVFilterContext *ctx, int64_t pts, unsigned flags);
```

## AvDevice

```c++
/// @brief utils
unsigned avdevice_version(void);
const char *avdevice_configuration(void);
const char *avdevice_license(void);
void avdevice_register_all(void);

/// @brief iterators
AVInputFormat *av_input_audio_device_next(AVInputFormat  *d);
AVInputFormat *av_input_video_device_next(AVInputFormat  *d);
AVOutputFormat *av_output_audio_device_next(AVOutputFormat *d);
AVOutputFormat *av_output_video_device_next(AVOutputFormat *d);

/// @brief List/Free devices.
/// @param [in] device context.
/// @param [out] list of autodetected devices.
/// @return count of autodetected devices, negative on error.
int avdevice_list_devices(struct AVFormatContext *s, AVDeviceInfoList **device_list);
void avdevice_free_list_devices(AVDeviceInfoList **device_list);
```

## SwScale

```c++
/// @brief utils
unsigned swscale_version(void);
const char *swscale_configuration(void);
const char *swscale_license(void);

/// @brief Return a positive value if pix_fmt is a supported input format, 0 otherwise
int sws_isSupportedInput(enum AVPixelFormat pix_fmt);

/// @brief Return a positive value if pix_fmt is a supported output format, 0 otherwise
int sws_isSupportedOutput(enum AVPixelFormat pix_fmt);

/// @brief Allocate and return an SwsContext.
/// @return a pointer to an allocated context, or NULL in case of error
struct SwsContext *sws_getContext(int srcW, int srcH, enum AVPixelFormat srcFormat,
                                  int dstW, int dstH, enum AVPixelFormat dstFormat,
                                  int flags, SwsFilter *srcFilter,
                                  SwsFilter *dstFilter, const double *param);

/// @brief Allocate/Free an empty SwsContext.
void sws_freeContext(struct SwsContext *swsContext);

/// @brief Scale the image slice in srcSlice and put the resulting scaled slice in the image in dst.
/// @return the height of the output slice
int sws_scale(struct SwsContext *c, const uint8_t *const srcSlice[],
              const int srcStride[], int srcSliceY, int srcSliceH,
              uint8_t *const dst[], const int dstStride[]);
```

## SwResample

```c++
/// @brief utils
unsigned swresample_version(void);
const char *swresample_configuration(void);
const char *swresample_license(void);

/// @brief Allocate SwrContext if needed and set/reset common parameters.
struct SwrContext *swr_alloc_set_opts(struct SwrContext *s,
                                      int64_t out_ch_layout, 
                                      enum AVSampleFormat out_sample_fmt, 
                                      int out_sample_rate, 
                                      int64_t  in_ch_layout, 
                                      enum AVSampleFormat  in_sample_fmt, 
                                      int  in_sample_rate,
                                      int log_offset, 
                                      void *log_ctx);

/// @brief Free the given SwrContext and set the pointer to NULL.
void swr_free(struct SwrContext **s);

/// @brief Convert audio
/// @return number of samples output per channel, negative value on error
/// @note in and in_count can be set to 0 to flush the last few samples out at the end
int swr_convert(struct SwrContext *s, uint8_t **out, int out_count,
                                const uint8_t **in , int in_count);

/// @brief Convert the samples in the input AVFrame and write them to the output AVFrame.
int swr_convert_frame(SwrContext *swr,
                      AVFrame *output, const AVFrame *input);
```

## AvUtils

### audio_fifo

```c++
/// @brief Allocate an AVAudioFifo
/// @return ewly allocated AVAudioFifo, or NULL on error
AVAudioFifo *av_audio_fifo_alloc(enum AVSampleFormat sample_fmt, int channels,
                                 int nb_samples);
void av_audio_fifo_free(AVAudioFifo *af);

/// @brief Reallocate an AVAudioFifo.
/// @return 0 if OK, or negative AVERROR code on failure
int av_audio_fifo_realloc(AVAudioFifo *af, int nb_samples);

/// @brief Write data to an AVAudioFifo.
/// @return number of samples actually written, or negative AVERROR code on failure.
/// @note The AVAudioFifo will be reallocated automatically if the available space is less than nb_samples.
int av_audio_fifo_write(AVAudioFifo *af, void **data, int nb_samples);

/// @brief Read data from an AVAudioFifo.
/// @return number of samples actually read, or negative AVERROR code on failure.
int av_audio_fifo_read(AVAudioFifo *af, void **data, int nb_samples);

/// @brief Peek(窥探) data from an AVAudioFifo.
/// @return number of samples actually peek, or negative AVERROR code on failure
int av_audio_fifo_peek(AVAudioFifo *af, void **data, int nb_samples);
int av_audio_fifo_peek_at(AVAudioFifo *af, void **data, int nb_samples, int offset);

/// @brief Removes the data without reading it.
/// @return 0 if OK, or negative AVERROR code on failure
int av_audio_fifo_drain(AVAudioFifo *af, int nb_samples);

/// @brief Reset the AVAudioFifo buffer. This empties all data in the buffer.
void av_audio_fifo_reset(AVAudioFifo *af);

/// @brief Get the current number of samples in the AVAudioFifo available for reading.
int av_audio_fifo_size(AVAudioFifo *af);

/// @brief Get the current number of samples in the AVAudioFifo available for writing.
int av_audio_fifo_space(AVAudioFifo *af);
```

### avutil

```c++
enum AVMediaType {
    AVMEDIA_TYPE_UNKNOWN = -1,  ///< Usually treated as AVMEDIA_TYPE_DATA
    AVMEDIA_TYPE_VIDEO,
    AVMEDIA_TYPE_AUDIO,
    AVMEDIA_TYPE_DATA,          ///< Opaque data information usually continuous
    AVMEDIA_TYPE_SUBTITLE,
    AVMEDIA_TYPE_ATTACHMENT,    ///< Opaque data information usually sparse
    AVMEDIA_TYPE_NB
};

enum AVPictureType {
    AV_PICTURE_TYPE_NONE = 0, ///< Undefined
    AV_PICTURE_TYPE_I,     ///< Intra
    AV_PICTURE_TYPE_P,     ///< Predicted
    AV_PICTURE_TYPE_B,     ///< Bi-dir predicted
    AV_PICTURE_TYPE_S,     ///< S(GMC)-VOP MPEG-4
    AV_PICTURE_TYPE_SI,    ///< Switching Intra
    AV_PICTURE_TYPE_SP,    ///< Switching Predicted
    AV_PICTURE_TYPE_BI,    ///< BI type
};

const char *av_get_media_type_string(enum AVMediaType media_type);
char av_get_picture_type_char(enum AVPictureType pict_type);

#define AV_NOPTS_VALUE          ((int64_t)UINT64_C(0x8000000000000000))
#define AV_TIME_BASE            1000000
#define AV_TIME_BASE_Q          (AVRational){1, AV_TIME_BASE}

/// @brief Return the fractional representation of the internal time base.
AVRational av_get_time_base_q(void);
```

### channel_layout

```c++
#define AV_CH_LAYOUT_MONO              (AV_CH_FRONT_CENTER)
#define AV_CH_LAYOUT_STEREO            (AV_CH_FRONT_LEFT|AV_CH_FRONT_RIGHT)
#define AV_CH_LAYOUT_2POINT1           (AV_CH_LAYOUT_STEREO|AV_CH_LOW_FREQUENCY)
#define AV_CH_LAYOUT_2_1               (AV_CH_LAYOUT_STEREO|AV_CH_BACK_CENTER)
#define AV_CH_LAYOUT_SURROUND          (AV_CH_LAYOUT_STEREO|AV_CH_FRONT_CENTER)
#define AV_CH_LAYOUT_3POINT1           (AV_CH_LAYOUT_SURROUND|AV_CH_LOW_FREQUENCY)
#define AV_CH_LAYOUT_4POINT0           (AV_CH_LAYOUT_SURROUND|AV_CH_BACK_CENTER)
#define AV_CH_LAYOUT_4POINT1           (AV_CH_LAYOUT_4POINT0|AV_CH_LOW_FREQUENCY)
#define AV_CH_LAYOUT_2_2               (AV_CH_LAYOUT_STEREO|AV_CH_SIDE_LEFT|AV_CH_SIDE_RIGHT)
#define AV_CH_LAYOUT_QUAD              (AV_CH_LAYOUT_STEREO|AV_CH_BACK_LEFT|AV_CH_BACK_RIGHT)
#define AV_CH_LAYOUT_5POINT0           (AV_CH_LAYOUT_SURROUND|AV_CH_SIDE_LEFT|AV_CH_SIDE_RIGHT)
#define AV_CH_LAYOUT_5POINT1           (AV_CH_LAYOUT_5POINT0|AV_CH_LOW_FREQUENCY)
#define AV_CH_LAYOUT_5POINT0_BACK      (AV_CH_LAYOUT_SURROUND|AV_CH_BACK_LEFT|AV_CH_BACK_RIGHT)
#define AV_CH_LAYOUT_5POINT1_BACK      (AV_CH_LAYOUT_5POINT0_BACK|AV_CH_LOW_FREQUENCY)
#define AV_CH_LAYOUT_6POINT0           (AV_CH_LAYOUT_5POINT0|AV_CH_BACK_CENTER)
#define AV_CH_LAYOUT_6POINT0_FRONT     (AV_CH_LAYOUT_2_2|AV_CH_FRONT_LEFT_OF_CENTER|AV_CH_FRONT_RIGHT_OF_CENTER)
#define AV_CH_LAYOUT_HEXAGONAL         (AV_CH_LAYOUT_5POINT0_BACK|AV_CH_BACK_CENTER)
#define AV_CH_LAYOUT_6POINT1           (AV_CH_LAYOUT_5POINT1|AV_CH_BACK_CENTER)
#define AV_CH_LAYOUT_6POINT1_BACK      (AV_CH_LAYOUT_5POINT1_BACK|AV_CH_BACK_CENTER)
#define AV_CH_LAYOUT_6POINT1_FRONT     (AV_CH_LAYOUT_6POINT0_FRONT|AV_CH_LOW_FREQUENCY)
#define AV_CH_LAYOUT_7POINT0           (AV_CH_LAYOUT_5POINT0|AV_CH_BACK_LEFT|AV_CH_BACK_RIGHT)
#define AV_CH_LAYOUT_7POINT0_FRONT     (AV_CH_LAYOUT_5POINT0|AV_CH_FRONT_LEFT_OF_CENTER|AV_CH_FRONT_RIGHT_OF_CENTER)
#define AV_CH_LAYOUT_7POINT1           (AV_CH_LAYOUT_5POINT1|AV_CH_BACK_LEFT|AV_CH_BACK_RIGHT)
#define AV_CH_LAYOUT_7POINT1_WIDE      (AV_CH_LAYOUT_5POINT1|AV_CH_FRONT_LEFT_OF_CENTER|AV_CH_FRONT_RIGHT_OF_CENTER)
#define AV_CH_LAYOUT_7POINT1_WIDE_BACK (AV_CH_LAYOUT_5POINT1_BACK|AV_CH_FRONT_LEFT_OF_CENTER|AV_CH_FRONT_RIGHT_OF_CENTER)
#define AV_CH_LAYOUT_OCTAGONAL         (AV_CH_LAYOUT_5POINT0|AV_CH_BACK_LEFT|AV_CH_BACK_CENTER|AV_CH_BACK_RIGHT)
#define AV_CH_LAYOUT_HEXADECAGONAL     (AV_CH_LAYOUT_OCTAGONAL|AV_CH_WIDE_LEFT|AV_CH_WIDE_RIGHT|AV_CH_TOP_BACK_LEFT|AV_CH_TOP_BACK_RIGHT|AV_CH_TOP_BACK_CENTER|AV_CH_TOP_FRONT_CENTER|AV_CH_TOP_FRONT_LEFT|AV_CH_TOP_FRONT_RIGHT)
#define AV_CH_LAYOUT_STEREO_DOWNMIX    (AV_CH_STEREO_LEFT|AV_CH_STEREO_RIGHT)
#define AV_CH_LAYOUT_22POINT2          (AV_CH_LAYOUT_5POINT1_BACK|AV_CH_FRONT_LEFT_OF_CENTER|AV_CH_FRONT_RIGHT_OF_CENTER|AV_CH_BACK_CENTER|AV_CH_LOW_FREQUENCY_2|AV_CH_SIDE_LEFT|AV_CH_SIDE_RIGHT|AV_CH_TOP_FRONT_LEFT|AV_CH_TOP_FRONT_RIGHT|AV_CH_TOP_FRONT_CENTER|AV_CH_TOP_CENTER|AV_CH_TOP_BACK_LEFT|AV_CH_TOP_BACK_RIGHT|AV_CH_TOP_SIDE_LEFT|AV_CH_TOP_SIDE_RIGHT|AV_CH_TOP_BACK_CENTER|AV_CH_BOTTOM_FRONT_CENTER|AV_CH_BOTTOM_FRONT_LEFT|AV_CH_BOTTOM_FRONT_RIGHT)

/// @brief utils
uint64_t av_get_channel_layout(const char *name);
int av_get_channel_layout_nb_channels(uint64_t channel_layout);
```

### common

```c++
#define FFUDIV(a,b) (((a)>0 ?(a):(a)-(b)+1) / (b))
#define FFUMOD(a,b) ((a)-(b)*FFUDIV(a,b))

#define FFABS(a) ((a) >= 0 ? (a) : (-(a)))
#define FFNABS(a) ((a) <= 0 ? (a) : (-(a)))
#define FFSIGN(a) ((a) > 0 ? 1 : -1)

#define FFMAX(a,b) ((a) > (b) ? (a) : (b))
#define FFMAX3(a,b,c) FFMAX(FFMAX(a,b),c)
#define FFMIN(a,b) ((a) > (b) ? (b) : (a))
#define FFMIN3(a,b,c) FFMIN(FFMIN(a,b),c)

/// @brief Clip a signed integer value into the amin-amax range.
int av_clip_c(int a, int amin, int amax);
float av_clipf_c(float a, float amin, float amax);
double av_clipd_c(double a, double amin, double amax);
```

### error

```c++
/// @brief Put a description of the AVERROR code errnum in errbuf.
int av_strerror(int errnum, char *errbuf, size_t errbuf_size);

/// @brief Fill the provided buffer with a string containing an error string corresponding to the AVERROR code errnum.
static inline char *av_make_error_string(char *errbuf, size_t errbuf_size, int errnum) {
    av_strerror(errnum, errbuf, errbuf_size);
    return errbuf;
}

/// @brief Convenience macro, the return value should be used only directly in function arguments but never stand-alone.
#define av_err2str(errnum) \
    av_make_error_string((char[AV_ERROR_MAX_STRING_SIZE]){0}, AV_ERROR_MAX_STRING_SIZE, errnum)
```

### avframe

```c++
typedef struct AVFrame {
#define AV_NUM_DATA_POINTERS 8
    /**
     * pointer to the picture/channel planes.
     * This might be different from the first allocated byte
     *
     * Some decoders access areas outside 0,0 - width,height, please
     * see avcodec_align_dimensions2(). Some filters and swscale can read
     * up to 16 bytes beyond the planes, if these filters are to be used,
     * then 16 extra bytes must be allocated.
     *
     * NOTE: Except for hwaccel formats, pointers not needed by the format
     * MUST be set to NULL.
     */
    uint8_t *data[AV_NUM_DATA_POINTERS];

    /**
     * For video, size in bytes of each picture line.
     * For audio, size in bytes of each plane.
     *
     * For audio, only linesize[0] may be set. For planar audio, each channel
     * plane must be the same size.
     *
     * For video the linesizes should be multiples of the CPUs alignment
     * preference, this is 16 or 32 for modern desktop CPUs.
     * Some code requires such alignment other code can be slower without
     * correct alignment, for yet other it makes no difference.
     *
     * @note The linesize may be larger than the size of usable data -- there
     * may be extra padding present for performance reasons.
     */
    int linesize[AV_NUM_DATA_POINTERS];

    /**
     * pointers to the data planes/channels.
     *
     * For video, this should simply point to data[].
     *
     * For planar audio, each channel has a separate data pointer, and
     * linesize[0] contains the size of each channel buffer.
     * For packed audio, there is just one data pointer, and linesize[0]
     * contains the total size of the buffer for all channels.
     *
     * Note: Both data and extended_data should always be set in a valid frame,
     * but for planar audio with more channels that can fit in data,
     * extended_data must be used in order to access all channels.
     */
    uint8_t **extended_data;

    /**
     * @name Video dimensions
     * Video frames only. The coded dimensions (in pixels) of the video frame,
     * i.e. the size of the rectangle that contains some well-defined values.
     *
     * @note The part of the frame intended for display/presentation is further
     * restricted by the @ref cropping "Cropping rectangle".
     * @{
     */
    int width, height;
    /**
     * @}
     */

    /**
     * number of audio samples (per channel) described by this frame
     */
    int nb_samples;

    /**
     * format of the frame, -1 if unknown or unset
     * Values correspond to enum AVPixelFormat for video frames,
     * enum AVSampleFormat for audio)
     */
    int format;

    /**
     * 1 -> keyframe, 0-> not
     */
    int key_frame;

    /**
     * Picture type of the frame.
     */
    enum AVPictureType pict_type;

    /**
     * Sample aspect ratio for the video frame, 0/1 if unknown/unspecified.
     */
  	// 视频样本的纵横比
    AVRational sample_aspect_ratio;

    /**
     * Presentation timestamp in time_base units (time when frame should be shown to user).
     */
    int64_t pts;

    /**
     * DTS copied from the AVPacket that triggered returning this frame. (if frame threading isn't used)
     * This is also the Presentation time of this AVFrame calculated from
     * only AVPacket.dts values without pts values.
     */
    int64_t pkt_dts;

    /**
     * picture number in bitstream order
     */
  	// 按比特流顺序排列的图片编号
    int coded_picture_number;
    /**
     * picture number in display order
     */
  	// 按播放顺序排列的图片编号
    int display_picture_number;

    /**
     * quality (between 1 (good) and FF_LAMBDA_MAX (bad))
     */
    int quality;

    /**
     * for some private data of the user
     */
    void *opaque;

    /**
     * When decoding, this signals how much the picture must be delayed.
     * extra_delay = repeat_pict / (2*fps)
     */
    int repeat_pict;

    /**
     * The content of the picture is interlaced（交错）.
     */
    int interlaced_frame;

    /**
     * If the content is interlaced, is top field displayed first.
     */
    int top_field_first;

    /**
     * Tell user application that palette has changed from previous frame.
     */
  	// 告诉用户应用程序调色板已从前一帧更改。
    int palette_has_changed;

    /**
     * reordered opaque 64 bits (generally an integer or a double precision float
     * PTS but can be anything).
     * The user sets AVCodecContext.reordered_opaque to represent the input at
     * that time,
     * the decoder reorders values as needed and sets AVFrame.reordered_opaque
     * to exactly one of the values provided by the user through AVCodecContext.reordered_opaque
     */
    int64_t reordered_opaque;

    /**
     * Sample rate of the audio data.
     */
    int sample_rate;

    /**
     * Channel layout of the audio data.
     */
    uint64_t channel_layout;

    /**
     * AVBuffer references backing the data for this frame. If all elements of
     * this array are NULL, then this frame is not reference counted. This array
     * must be filled contiguously -- if buf[i] is non-NULL then buf[j] must
     * also be non-NULL for all j < i.
     *
     * There may be at most one AVBuffer per data plane, so for video this array
     * always contains all the references. For planar audio with more than
     * AV_NUM_DATA_POINTERS channels, there may be more buffers than can fit in
     * this array. Then the extra AVBufferRef pointers are stored in the
     * extended_buf array.
     */
    AVBufferRef *buf[AV_NUM_DATA_POINTERS];

    /**
     * For planar audio which requires more than AV_NUM_DATA_POINTERS
     * AVBufferRef pointers, this array will hold all the references which
     * cannot fit into AVFrame.buf.
     *
     * Note that this is different from AVFrame.extended_data, which always
     * contains all the pointers. This array only contains the extra pointers,
     * which cannot fit into AVFrame.buf.
     *
     * This array is always allocated using av_malloc() by whoever constructs
     * the frame. It is freed in av_frame_unref().
     */
    AVBufferRef **extended_buf;
    /**
     * Number of elements in extended_buf.
     */
    int        nb_extended_buf;

    AVFrameSideData **side_data;
    int            nb_side_data;

/**
 * @defgroup lavu_frame_flags AV_FRAME_FLAGS
 * @ingroup lavu_frame
 * Flags describing additional frame properties.
 *
 * @{
 */

/**
 * The frame data may be corrupted, e.g. due to decoding errors.
 */
#define AV_FRAME_FLAG_CORRUPT       (1 << 0)
/**
 * A flag to mark the frames which need to be decoded, but shouldn't be output.
 */
#define AV_FRAME_FLAG_DISCARD   (1 << 2)
/**
 * @}
 */

    /**
     * Frame flags, a combination of @ref lavu_frame_flags
     */
    int flags;

    /**
     * MPEG vs JPEG YUV range.
     * - encoding: Set by user
     * - decoding: Set by libavcodec
     */
    enum AVColorRange color_range;

    enum AVColorPrimaries color_primaries;

    enum AVColorTransferCharacteristic color_trc;

    /**
     * YUV colorspace type.
     * - encoding: Set by user
     * - decoding: Set by libavcodec
     */
    enum AVColorSpace colorspace;

    enum AVChromaLocation chroma_location;

    /**
     * frame timestamp estimated using various heuristics, in stream time base
     * - encoding: unused
     * - decoding: set by libavcodec, read by user.
     */
    int64_t best_effort_timestamp;

    /**
     * reordered pos from the last AVPacket that has been input into the decoder
     * - encoding: unused
     * - decoding: Read by user.
     */
    int64_t pkt_pos;

    /**
     * duration of the corresponding packet, expressed in
     * AVStream->time_base units, 0 if unknown.
     * - encoding: unused
     * - decoding: Read by user.
     */
    int64_t pkt_duration;

    /**
     * metadata.
     * - encoding: Set by user.
     * - decoding: Set by libavcodec.
     */
    AVDictionary *metadata;

    /**
     * decode error flags of the frame, set to a combination of
     * FF_DECODE_ERROR_xxx flags if the decoder produced a frame, but there
     * were errors during the decoding.
     * - encoding: unused
     * - decoding: set by libavcodec, read by user.
     */
    int decode_error_flags;
#define FF_DECODE_ERROR_INVALID_BITSTREAM   1
#define FF_DECODE_ERROR_MISSING_REFERENCE   2
#define FF_DECODE_ERROR_CONCEALMENT_ACTIVE  4
#define FF_DECODE_ERROR_DECODE_SLICES       8

    /**
     * number of audio channels, only used for audio.
     * - encoding: unused
     * - decoding: Read by user.
     */
    int channels;

    /**
     * size of the corresponding packet containing the compressed
     * frame.
     * It is set to a negative value if unknown.
     * - encoding: unused
     * - decoding: set by libavcodec, read by user.
     */
    int pkt_size;

    /**
     * For hwaccel-format frames, this should be a reference to the
     * AVHWFramesContext describing the frame.
     */
    AVBufferRef *hw_frames_ctx;

    /**
     * AVBufferRef for free use by the API user. FFmpeg will never check the
     * contents of the buffer ref. FFmpeg calls av_buffer_unref() on it when
     * the frame is unreferenced. av_frame_copy_props() calls create a new
     * reference with av_buffer_ref() for the target frame's opaque_ref field.
     *
     * This is unrelated to the opaque field, although it serves a similar
     * purpose.
     */
    AVBufferRef *opaque_ref;

    /**
     * @anchor cropping
     * @name Cropping
     * Video frames only. The number of pixels to discard from the the
     * top/bottom/left/right border of the frame to obtain the sub-rectangle of
     * the frame intended for presentation.
     * @{
     */
    size_t crop_top;
    size_t crop_bottom;
    size_t crop_left;
    size_t crop_right;
    /**
     * @}
     */

    /**
     * AVBufferRef for internal use by a single libav* library.
     * Must not be used to transfer data between libraries.
     * Has to be NULL when ownership of the frame leaves the respective library.
     *
     * Code outside the FFmpeg libs should never check or change the contents of the buffer ref.
     *
     * FFmpeg calls av_buffer_unref() on it when the frame is unreferenced.
     * av_frame_copy_props() calls create a new reference with av_buffer_ref()
     * for the target frame's private_ref field.
     */
    AVBufferRef *private_ref;
} AVFrame;

/// @brief Allocate/Free an AVFrame and set its fields to default values.
AVFrame *av_frame_alloc(void);
void av_frame_free(AVFrame **frame);

/// @brief Set up a new reference to the data described by the source frame.
int av_frame_ref(AVFrame *dst, const AVFrame *src);
void av_frame_unref(AVFrame *frame);

/// @brief Create a new frame that references the same data as src. This is a shortcut for av_frame_alloc()+av_frame_ref().
AVFrame *av_frame_clone(const AVFrame *src);

/// @brief Move everything contained in src to dst and reset src.
void av_frame_move_ref(AVFrame *dst, AVFrame *src);

/// @brief Allocate new buffer(s) for audio or video data.
int av_frame_get_buffer(AVFrame *frame, int align);

/// @brief Check if the frame data is writable.
int av_frame_is_writable(AVFrame *frame);
int av_frame_make_writable(AVFrame *frame);

/// @brief Copy the frame data from src to dst.
/// @note This function only copies the frame data, not any other properties.
int av_frame_copy(AVFrame *dst, const AVFrame *src);

/// @brief Copy only "metadata" fields from src to dst.
int av_frame_copy_props(AVFrame *dst, const AVFrame *src);

/// @brief Crop the given video AVFrame according to its crop_left/crop_top/crop_right/crop_bottom fields. Some combination of AV_FRAME_CROP_* flags, or 0.
int av_frame_apply_cropping(AVFrame *frame, int flags);
```

### hwcontext

```c++
enum AVHWDeviceType {
    AV_HWDEVICE_TYPE_NONE,
    AV_HWDEVICE_TYPE_VDPAU,
    AV_HWDEVICE_TYPE_CUDA,
    AV_HWDEVICE_TYPE_VAAPI,
    AV_HWDEVICE_TYPE_DXVA2,
    AV_HWDEVICE_TYPE_QSV,
    AV_HWDEVICE_TYPE_VIDEOTOOLBOX,
    AV_HWDEVICE_TYPE_D3D11VA,
    AV_HWDEVICE_TYPE_DRM,
    AV_HWDEVICE_TYPE_OPENCL,
    AV_HWDEVICE_TYPE_MEDIACODEC,
    AV_HWDEVICE_TYPE_VULKAN,
};


// ....
// 硬件解码相关的，遇到了再说
```

### imageutils

```c++
/// @brief Compute the max pixel step for each plane of an image with a format described by pixdesc.
void av_image_fill_max_pixsteps(int max_pixsteps[4], int max_pixstep_comps[4],
                                const AVPixFmtDescriptor *pixdesc);

/// @brief Compute the size of an image line with format pix_fmt and width width for the plane plane.
int av_image_get_linesize(enum AVPixelFormat pix_fmt, int width, int plane);

/// @brief Fill plane linesizes for an image with pixel format pix_fmt and width width.
int av_image_fill_linesizes(int linesizes[4], enum AVPixelFormat pix_fmt, int width);

/// @brief Fill plane sizes for an image with pixel format pix_fmt and height height.
int av_image_fill_plane_sizes(size_t size[4], enum AVPixelFormat pix_fmt,
                              int height, const ptrdiff_t linesizes[4]);

/// @brief Fill plane data pointers for an image with pixel format pix_fmt and height height.
int av_image_fill_pointers(uint8_t *data[4], enum AVPixelFormat pix_fmt, int height,
                           uint8_t *ptr, const int linesizes[4]);

/// @brief Allocate an image with size w and h and pixel format pix_fmt, and fill pointers and linesizes accordingly.
int av_image_alloc(uint8_t *pointers[4], int linesizes[4], int w, int h, enum AVPixelFormat pix_fmt, int align);

/// @brief Copy image plane from src to dst.
void av_image_copy_plane(uint8_t *dst, int dst_linesize, const uint8_t *src, int src_linesize, int bytewidth, int height);

/// @brief Copy image in src_data to dst_data.
void av_image_copy(uint8_t *dst_data[4], int dst_linesizes[4],
                   const uint8_t *src_data[4], const int src_linesizes[4],
                   enum AVPixelFormat pix_fmt, int width, int height);

/// @brief Return the size in bytes of the amount of data required to store an image with the given parameters.
int av_image_get_buffer_size(enum AVPixelFormat pix_fmt, int width, int height, int align);

/// @brief Copy image data from an image into a buffer.
int av_image_copy_to_buffer(uint8_t *dst, int dst_size,
                            const uint8_t * const src_data[4], const int src_linesize[4],
                            enum AVPixelFormat pix_fmt, int width, int height, int align);

/// @brief Overwrite the image data with black.
/// @param [in] pix_fmt: the pixel format of the image
/// @param [in] range: the color range of the image (important for colorspaces such as YUV) 
int av_image_fill_black(uint8_t *dst_data[4], const ptrdiff_t dst_linesize[4],
                        enum AVPixelFormat pix_fmt, enum AVColorRange range,
                        int width, int height);
```

### mathematics

```c++
enum AVRounding {
    AV_ROUND_ZERO     = 0, ///< Round toward zero.
    AV_ROUND_INF      = 1, ///< Round away from zero.
    AV_ROUND_DOWN     = 2, ///< Round toward -infinity.
    AV_ROUND_UP       = 3, ///< Round toward +infinity.
    AV_ROUND_NEAR_INF = 5, ///< Round to nearest and halfway cases away from zero.
    /**
     * Flag telling rescaling functions to pass `INT64_MIN`/`MAX` through
     * unchanged, avoiding special cases for #AV_NOPTS_VALUE.
     *
     * Unlike other values of the enumeration AVRounding, this value is a
     * bitmask that must be used in conjunction with another value of the
     * enumeration through a bitwise OR, in order to set behavior for normal
     * cases.
     *
     * @code{.c}
     * av_rescale_rnd(3, 1, 2, AV_ROUND_UP | AV_ROUND_PASS_MINMAX);
     * // Rescaling 3:
     * //     Calculating 3 * 1 / 2
     * //     3 / 2 is rounded up to 2
     * //     => 2
     *
     * av_rescale_rnd(AV_NOPTS_VALUE, 1, 2, AV_ROUND_UP | AV_ROUND_PASS_MINMAX);
     * // Rescaling AV_NOPTS_VALUE:
     * //     AV_NOPTS_VALUE == INT64_MIN
     * //     AV_NOPTS_VALUE is passed through
     * //     => AV_NOPTS_VALUE
     * @endcode
     */
    AV_ROUND_PASS_MINMAX = 8192,
};

/// @brief a * b / c
int64_t av_rescale(int64_t a, int64_t b, int64_t c) av_const;

///@brief a * b / c
int64_t av_rescale_rnd(int64_t a, int64_t b, int64_t c, enum AVRounding rnd) av_const;

/// @brief a * bq / cq
int64_t av_rescale_q(int64_t a, AVRational bq, AVRational cq) av_const;

/// @brief a * bq / cq
int64_t av_rescale_q_rnd(int64_t a, AVRational bq, AVRational cq,
                         enum AVRounding rnd) av_const;
```

### pixfmt

```c++
enum AVPixelFormat {
    AV_PIX_FMT_NONE = -1,
    AV_PIX_FMT_YUV420P,   ///< planar YUV 4:2:0, 12bpp, (1 Cr & Cb sample per 2x2 Y samples)
    AV_PIX_FMT_YUYV422,   ///< packed YUV 4:2:2, 16bpp, Y0 Cb Y1 Cr
    AV_PIX_FMT_RGB24,     ///< packed RGB 8:8:8, 24bpp, RGBRGB...
    AV_PIX_FMT_BGR24,     ///< packed RGB 8:8:8, 24bpp, BGRBGR...
    AV_PIX_FMT_YUV422P,   ///< planar YUV 4:2:2, 16bpp, (1 Cr & Cb sample per 2x1 Y samples)
    AV_PIX_FMT_YUV444P,   ///< planar YUV 4:4:4, 24bpp, (1 Cr & Cb sample per 1x1 Y samples)
    AV_PIX_FMT_YUV410P,   ///< planar YUV 4:1:0,  9bpp, (1 Cr & Cb sample per 4x4 Y samples)
    AV_PIX_FMT_YUV411P,   ///< planar YUV 4:1:1, 12bpp, (1 Cr & Cb sample per 4x1 Y samples)
    AV_PIX_FMT_GRAY8,     ///<        Y        ,  8bpp
    AV_PIX_FMT_MONOWHITE, ///<        Y        ,  1bpp, 0 is white, 1 is black, in each byte pixels are ordered from the msb to the lsb
    AV_PIX_FMT_MONOBLACK, ///<        Y        ,  1bpp, 0 is black, 1 is white, in each byte pixels are ordered from the msb to the lsb
    AV_PIX_FMT_PAL8,      ///< 8 bits with AV_PIX_FMT_RGB32 palette
    AV_PIX_FMT_YUVJ420P,  ///< planar YUV 4:2:0, 12bpp, full scale (JPEG), deprecated in favor of AV_PIX_FMT_YUV420P and setting color_range
    AV_PIX_FMT_YUVJ422P,  ///< planar YUV 4:2:2, 16bpp, full scale (JPEG), deprecated in favor of AV_PIX_FMT_YUV422P and setting color_range
    AV_PIX_FMT_YUVJ444P,  ///< planar YUV 4:4:4, 24bpp, full scale (JPEG), deprecated in favor of AV_PIX_FMT_YUV444P and setting color_range
    AV_PIX_FMT_UYVY422,   ///< packed YUV 4:2:2, 16bpp, Cb Y0 Cr Y1
    AV_PIX_FMT_UYYVYY411, ///< packed YUV 4:1:1, 12bpp, Cb Y0 Y1 Cr Y2 Y3
    AV_PIX_FMT_BGR8,      ///< packed RGB 3:3:2,  8bpp, (msb)2B 3G 3R(lsb)
    AV_PIX_FMT_BGR4,      ///< packed RGB 1:2:1 bitstream,  4bpp, (msb)1B 2G 1R(lsb), a byte contains two pixels, the first pixel in the byte is the one composed by the 4 msb bits
    AV_PIX_FMT_BGR4_BYTE, ///< packed RGB 1:2:1,  8bpp, (msb)1B 2G 1R(lsb)
    AV_PIX_FMT_RGB8,      ///< packed RGB 3:3:2,  8bpp, (msb)2R 3G 3B(lsb)
    AV_PIX_FMT_RGB4,      ///< packed RGB 1:2:1 bitstream,  4bpp, (msb)1R 2G 1B(lsb), a byte contains two pixels, the first pixel in the byte is the one composed by the 4 msb bits
    AV_PIX_FMT_RGB4_BYTE, ///< packed RGB 1:2:1,  8bpp, (msb)1R 2G 1B(lsb)
    AV_PIX_FMT_NV12,      ///< planar YUV 4:2:0, 12bpp, 1 plane for Y and 1 plane for the UV components, which are interleaved (first byte U and the following byte V)
    AV_PIX_FMT_NV21,      ///< as above, but U and V bytes are swapped

    AV_PIX_FMT_ARGB,      ///< packed ARGB 8:8:8:8, 32bpp, ARGBARGB...
    AV_PIX_FMT_RGBA,      ///< packed RGBA 8:8:8:8, 32bpp, RGBARGBA...
    AV_PIX_FMT_ABGR,      ///< packed ABGR 8:8:8:8, 32bpp, ABGRABGR...
    AV_PIX_FMT_BGRA,      ///< packed BGRA 8:8:8:8, 32bpp, BGRABGRA...

    AV_PIX_FMT_GRAY16BE,  ///<        Y        , 16bpp, big-endian
    AV_PIX_FMT_GRAY16LE,  ///<        Y        , 16bpp, little-endian
    AV_PIX_FMT_YUV440P,   ///< planar YUV 4:4:0 (1 Cr & Cb sample per 1x2 Y samples)
    AV_PIX_FMT_YUVJ440P,  ///< planar YUV 4:4:0 full scale (JPEG), deprecated in favor of AV_PIX_FMT_YUV440P and setting color_range
    AV_PIX_FMT_YUVA420P,  ///< planar YUV 4:2:0, 20bpp, (1 Cr & Cb sample per 2x2 Y & A samples)
    AV_PIX_FMT_RGB48BE,   ///< packed RGB 16:16:16, 48bpp, 16R, 16G, 16B, the 2-byte value for each R/G/B component is stored as big-endian
    AV_PIX_FMT_RGB48LE,   ///< packed RGB 16:16:16, 48bpp, 16R, 16G, 16B, the 2-byte value for each R/G/B component is stored as little-endian

    AV_PIX_FMT_RGB565BE,  ///< packed RGB 5:6:5, 16bpp, (msb)   5R 6G 5B(lsb), big-endian
    AV_PIX_FMT_RGB565LE,  ///< packed RGB 5:6:5, 16bpp, (msb)   5R 6G 5B(lsb), little-endian
    AV_PIX_FMT_RGB555BE,  ///< packed RGB 5:5:5, 16bpp, (msb)1X 5R 5G 5B(lsb), big-endian   , X=unused/undefined
    AV_PIX_FMT_RGB555LE,  ///< packed RGB 5:5:5, 16bpp, (msb)1X 5R 5G 5B(lsb), little-endian, X=unused/undefined

    AV_PIX_FMT_BGR565BE,  ///< packed BGR 5:6:5, 16bpp, (msb)   5B 6G 5R(lsb), big-endian
    AV_PIX_FMT_BGR565LE,  ///< packed BGR 5:6:5, 16bpp, (msb)   5B 6G 5R(lsb), little-endian
    AV_PIX_FMT_BGR555BE,  ///< packed BGR 5:5:5, 16bpp, (msb)1X 5B 5G 5R(lsb), big-endian   , X=unused/undefined
    AV_PIX_FMT_BGR555LE,  ///< packed BGR 5:5:5, 16bpp, (msb)1X 5B 5G 5R(lsb), little-endian, X=unused/undefined

#if FF_API_VAAPI
    /** @name Deprecated pixel formats */
    /**@{*/
    AV_PIX_FMT_VAAPI_MOCO, ///< HW acceleration through VA API at motion compensation entry-point, Picture.data[3] contains a vaapi_render_state struct which contains macroblocks as well as various fields extracted from headers
    AV_PIX_FMT_VAAPI_IDCT, ///< HW acceleration through VA API at IDCT entry-point, Picture.data[3] contains a vaapi_render_state struct which contains fields extracted from headers
    AV_PIX_FMT_VAAPI_VLD,  ///< HW decoding through VA API, Picture.data[3] contains a VASurfaceID
    /**@}*/
    AV_PIX_FMT_VAAPI = AV_PIX_FMT_VAAPI_VLD,
#else
    /**
     *  Hardware acceleration through VA-API, data[3] contains a
     *  VASurfaceID.
     */
    AV_PIX_FMT_VAAPI,
#endif

    AV_PIX_FMT_YUV420P16LE,  ///< planar YUV 4:2:0, 24bpp, (1 Cr & Cb sample per 2x2 Y samples), little-endian
    AV_PIX_FMT_YUV420P16BE,  ///< planar YUV 4:2:0, 24bpp, (1 Cr & Cb sample per 2x2 Y samples), big-endian
    AV_PIX_FMT_YUV422P16LE,  ///< planar YUV 4:2:2, 32bpp, (1 Cr & Cb sample per 2x1 Y samples), little-endian
    AV_PIX_FMT_YUV422P16BE,  ///< planar YUV 4:2:2, 32bpp, (1 Cr & Cb sample per 2x1 Y samples), big-endian
    AV_PIX_FMT_YUV444P16LE,  ///< planar YUV 4:4:4, 48bpp, (1 Cr & Cb sample per 1x1 Y samples), little-endian
    AV_PIX_FMT_YUV444P16BE,  ///< planar YUV 4:4:4, 48bpp, (1 Cr & Cb sample per 1x1 Y samples), big-endian
    AV_PIX_FMT_DXVA2_VLD,    ///< HW decoding through DXVA2, Picture.data[3] contains a LPDIRECT3DSURFACE9 pointer

    AV_PIX_FMT_RGB444LE,  ///< packed RGB 4:4:4, 16bpp, (msb)4X 4R 4G 4B(lsb), little-endian, X=unused/undefined
    AV_PIX_FMT_RGB444BE,  ///< packed RGB 4:4:4, 16bpp, (msb)4X 4R 4G 4B(lsb), big-endian,    X=unused/undefined
    AV_PIX_FMT_BGR444LE,  ///< packed BGR 4:4:4, 16bpp, (msb)4X 4B 4G 4R(lsb), little-endian, X=unused/undefined
    AV_PIX_FMT_BGR444BE,  ///< packed BGR 4:4:4, 16bpp, (msb)4X 4B 4G 4R(lsb), big-endian,    X=unused/undefined
    AV_PIX_FMT_YA8,       ///< 8 bits gray, 8 bits alpha

    AV_PIX_FMT_Y400A = AV_PIX_FMT_YA8, ///< alias for AV_PIX_FMT_YA8
    AV_PIX_FMT_GRAY8A= AV_PIX_FMT_YA8, ///< alias for AV_PIX_FMT_YA8

    AV_PIX_FMT_BGR48BE,   ///< packed RGB 16:16:16, 48bpp, 16B, 16G, 16R, the 2-byte value for each R/G/B component is stored as big-endian
    AV_PIX_FMT_BGR48LE,   ///< packed RGB 16:16:16, 48bpp, 16B, 16G, 16R, the 2-byte value for each R/G/B component is stored as little-endian

    /**
     * The following 12 formats have the disadvantage of needing 1 format for each bit depth.
     * Notice that each 9/10 bits sample is stored in 16 bits with extra padding.
     * If you want to support multiple bit depths, then using AV_PIX_FMT_YUV420P16* with the bpp stored separately is better.
     */
    AV_PIX_FMT_YUV420P9BE, ///< planar YUV 4:2:0, 13.5bpp, (1 Cr & Cb sample per 2x2 Y samples), big-endian
    AV_PIX_FMT_YUV420P9LE, ///< planar YUV 4:2:0, 13.5bpp, (1 Cr & Cb sample per 2x2 Y samples), little-endian
    AV_PIX_FMT_YUV420P10BE,///< planar YUV 4:2:0, 15bpp, (1 Cr & Cb sample per 2x2 Y samples), big-endian
    AV_PIX_FMT_YUV420P10LE,///< planar YUV 4:2:0, 15bpp, (1 Cr & Cb sample per 2x2 Y samples), little-endian
    AV_PIX_FMT_YUV422P10BE,///< planar YUV 4:2:2, 20bpp, (1 Cr & Cb sample per 2x1 Y samples), big-endian
    AV_PIX_FMT_YUV422P10LE,///< planar YUV 4:2:2, 20bpp, (1 Cr & Cb sample per 2x1 Y samples), little-endian
    AV_PIX_FMT_YUV444P9BE, ///< planar YUV 4:4:4, 27bpp, (1 Cr & Cb sample per 1x1 Y samples), big-endian
    AV_PIX_FMT_YUV444P9LE, ///< planar YUV 4:4:4, 27bpp, (1 Cr & Cb sample per 1x1 Y samples), little-endian
    AV_PIX_FMT_YUV444P10BE,///< planar YUV 4:4:4, 30bpp, (1 Cr & Cb sample per 1x1 Y samples), big-endian
    AV_PIX_FMT_YUV444P10LE,///< planar YUV 4:4:4, 30bpp, (1 Cr & Cb sample per 1x1 Y samples), little-endian
    AV_PIX_FMT_YUV422P9BE, ///< planar YUV 4:2:2, 18bpp, (1 Cr & Cb sample per 2x1 Y samples), big-endian
    AV_PIX_FMT_YUV422P9LE, ///< planar YUV 4:2:2, 18bpp, (1 Cr & Cb sample per 2x1 Y samples), little-endian
    AV_PIX_FMT_GBRP,      ///< planar GBR 4:4:4 24bpp
    AV_PIX_FMT_GBR24P = AV_PIX_FMT_GBRP, // alias for #AV_PIX_FMT_GBRP
    AV_PIX_FMT_GBRP9BE,   ///< planar GBR 4:4:4 27bpp, big-endian
    AV_PIX_FMT_GBRP9LE,   ///< planar GBR 4:4:4 27bpp, little-endian
    AV_PIX_FMT_GBRP10BE,  ///< planar GBR 4:4:4 30bpp, big-endian
    AV_PIX_FMT_GBRP10LE,  ///< planar GBR 4:4:4 30bpp, little-endian
    AV_PIX_FMT_GBRP16BE,  ///< planar GBR 4:4:4 48bpp, big-endian
    AV_PIX_FMT_GBRP16LE,  ///< planar GBR 4:4:4 48bpp, little-endian
    AV_PIX_FMT_YUVA422P,  ///< planar YUV 4:2:2 24bpp, (1 Cr & Cb sample per 2x1 Y & A samples)
    AV_PIX_FMT_YUVA444P,  ///< planar YUV 4:4:4 32bpp, (1 Cr & Cb sample per 1x1 Y & A samples)
    AV_PIX_FMT_YUVA420P9BE,  ///< planar YUV 4:2:0 22.5bpp, (1 Cr & Cb sample per 2x2 Y & A samples), big-endian
    AV_PIX_FMT_YUVA420P9LE,  ///< planar YUV 4:2:0 22.5bpp, (1 Cr & Cb sample per 2x2 Y & A samples), little-endian
    AV_PIX_FMT_YUVA422P9BE,  ///< planar YUV 4:2:2 27bpp, (1 Cr & Cb sample per 2x1 Y & A samples), big-endian
    AV_PIX_FMT_YUVA422P9LE,  ///< planar YUV 4:2:2 27bpp, (1 Cr & Cb sample per 2x1 Y & A samples), little-endian
    AV_PIX_FMT_YUVA444P9BE,  ///< planar YUV 4:4:4 36bpp, (1 Cr & Cb sample per 1x1 Y & A samples), big-endian
    AV_PIX_FMT_YUVA444P9LE,  ///< planar YUV 4:4:4 36bpp, (1 Cr & Cb sample per 1x1 Y & A samples), little-endian
    AV_PIX_FMT_YUVA420P10BE, ///< planar YUV 4:2:0 25bpp, (1 Cr & Cb sample per 2x2 Y & A samples, big-endian)
    AV_PIX_FMT_YUVA420P10LE, ///< planar YUV 4:2:0 25bpp, (1 Cr & Cb sample per 2x2 Y & A samples, little-endian)
    AV_PIX_FMT_YUVA422P10BE, ///< planar YUV 4:2:2 30bpp, (1 Cr & Cb sample per 2x1 Y & A samples, big-endian)
    AV_PIX_FMT_YUVA422P10LE, ///< planar YUV 4:2:2 30bpp, (1 Cr & Cb sample per 2x1 Y & A samples, little-endian)
    AV_PIX_FMT_YUVA444P10BE, ///< planar YUV 4:4:4 40bpp, (1 Cr & Cb sample per 1x1 Y & A samples, big-endian)
    AV_PIX_FMT_YUVA444P10LE, ///< planar YUV 4:4:4 40bpp, (1 Cr & Cb sample per 1x1 Y & A samples, little-endian)
    AV_PIX_FMT_YUVA420P16BE, ///< planar YUV 4:2:0 40bpp, (1 Cr & Cb sample per 2x2 Y & A samples, big-endian)
    AV_PIX_FMT_YUVA420P16LE, ///< planar YUV 4:2:0 40bpp, (1 Cr & Cb sample per 2x2 Y & A samples, little-endian)
    AV_PIX_FMT_YUVA422P16BE, ///< planar YUV 4:2:2 48bpp, (1 Cr & Cb sample per 2x1 Y & A samples, big-endian)
    AV_PIX_FMT_YUVA422P16LE, ///< planar YUV 4:2:2 48bpp, (1 Cr & Cb sample per 2x1 Y & A samples, little-endian)
    AV_PIX_FMT_YUVA444P16BE, ///< planar YUV 4:4:4 64bpp, (1 Cr & Cb sample per 1x1 Y & A samples, big-endian)
    AV_PIX_FMT_YUVA444P16LE, ///< planar YUV 4:4:4 64bpp, (1 Cr & Cb sample per 1x1 Y & A samples, little-endian)

    AV_PIX_FMT_VDPAU,     ///< HW acceleration through VDPAU, Picture.data[3] contains a VdpVideoSurface

    AV_PIX_FMT_XYZ12LE,      ///< packed XYZ 4:4:4, 36 bpp, (msb) 12X, 12Y, 12Z (lsb), the 2-byte value for each X/Y/Z is stored as little-endian, the 4 lower bits are set to 0
    AV_PIX_FMT_XYZ12BE,      ///< packed XYZ 4:4:4, 36 bpp, (msb) 12X, 12Y, 12Z (lsb), the 2-byte value for each X/Y/Z is stored as big-endian, the 4 lower bits are set to 0
    AV_PIX_FMT_NV16,         ///< interleaved chroma YUV 4:2:2, 16bpp, (1 Cr & Cb sample per 2x1 Y samples)
    AV_PIX_FMT_NV20LE,       ///< interleaved chroma YUV 4:2:2, 20bpp, (1 Cr & Cb sample per 2x1 Y samples), little-endian
    AV_PIX_FMT_NV20BE,       ///< interleaved chroma YUV 4:2:2, 20bpp, (1 Cr & Cb sample per 2x1 Y samples), big-endian

    AV_PIX_FMT_RGBA64BE,     ///< packed RGBA 16:16:16:16, 64bpp, 16R, 16G, 16B, 16A, the 2-byte value for each R/G/B/A component is stored as big-endian
    AV_PIX_FMT_RGBA64LE,     ///< packed RGBA 16:16:16:16, 64bpp, 16R, 16G, 16B, 16A, the 2-byte value for each R/G/B/A component is stored as little-endian
    AV_PIX_FMT_BGRA64BE,     ///< packed RGBA 16:16:16:16, 64bpp, 16B, 16G, 16R, 16A, the 2-byte value for each R/G/B/A component is stored as big-endian
    AV_PIX_FMT_BGRA64LE,     ///< packed RGBA 16:16:16:16, 64bpp, 16B, 16G, 16R, 16A, the 2-byte value for each R/G/B/A component is stored as little-endian

    AV_PIX_FMT_YVYU422,   ///< packed YUV 4:2:2, 16bpp, Y0 Cr Y1 Cb

    AV_PIX_FMT_YA16BE,       ///< 16 bits gray, 16 bits alpha (big-endian)
    AV_PIX_FMT_YA16LE,       ///< 16 bits gray, 16 bits alpha (little-endian)

    AV_PIX_FMT_GBRAP,        ///< planar GBRA 4:4:4:4 32bpp
    AV_PIX_FMT_GBRAP16BE,    ///< planar GBRA 4:4:4:4 64bpp, big-endian
    AV_PIX_FMT_GBRAP16LE,    ///< planar GBRA 4:4:4:4 64bpp, little-endian
    /**
     *  HW acceleration through QSV, data[3] contains a pointer to the
     *  mfxFrameSurface1 structure.
     */
    AV_PIX_FMT_QSV,
    /**
     * HW acceleration though MMAL, data[3] contains a pointer to the
     * MMAL_BUFFER_HEADER_T structure.
     */
    AV_PIX_FMT_MMAL,

    AV_PIX_FMT_D3D11VA_VLD,  ///< HW decoding through Direct3D11 via old API, Picture.data[3] contains a ID3D11VideoDecoderOutputView pointer

    /**
     * HW acceleration through CUDA. data[i] contain CUdeviceptr pointers
     * exactly as for system memory frames.
     */
    AV_PIX_FMT_CUDA,

    AV_PIX_FMT_0RGB,        ///< packed RGB 8:8:8, 32bpp, XRGBXRGB...   X=unused/undefined
    AV_PIX_FMT_RGB0,        ///< packed RGB 8:8:8, 32bpp, RGBXRGBX...   X=unused/undefined
    AV_PIX_FMT_0BGR,        ///< packed BGR 8:8:8, 32bpp, XBGRXBGR...   X=unused/undefined
    AV_PIX_FMT_BGR0,        ///< packed BGR 8:8:8, 32bpp, BGRXBGRX...   X=unused/undefined

    AV_PIX_FMT_YUV420P12BE, ///< planar YUV 4:2:0,18bpp, (1 Cr & Cb sample per 2x2 Y samples), big-endian
    AV_PIX_FMT_YUV420P12LE, ///< planar YUV 4:2:0,18bpp, (1 Cr & Cb sample per 2x2 Y samples), little-endian
    AV_PIX_FMT_YUV420P14BE, ///< planar YUV 4:2:0,21bpp, (1 Cr & Cb sample per 2x2 Y samples), big-endian
    AV_PIX_FMT_YUV420P14LE, ///< planar YUV 4:2:0,21bpp, (1 Cr & Cb sample per 2x2 Y samples), little-endian
    AV_PIX_FMT_YUV422P12BE, ///< planar YUV 4:2:2,24bpp, (1 Cr & Cb sample per 2x1 Y samples), big-endian
    AV_PIX_FMT_YUV422P12LE, ///< planar YUV 4:2:2,24bpp, (1 Cr & Cb sample per 2x1 Y samples), little-endian
    AV_PIX_FMT_YUV422P14BE, ///< planar YUV 4:2:2,28bpp, (1 Cr & Cb sample per 2x1 Y samples), big-endian
    AV_PIX_FMT_YUV422P14LE, ///< planar YUV 4:2:2,28bpp, (1 Cr & Cb sample per 2x1 Y samples), little-endian
    AV_PIX_FMT_YUV444P12BE, ///< planar YUV 4:4:4,36bpp, (1 Cr & Cb sample per 1x1 Y samples), big-endian
    AV_PIX_FMT_YUV444P12LE, ///< planar YUV 4:4:4,36bpp, (1 Cr & Cb sample per 1x1 Y samples), little-endian
    AV_PIX_FMT_YUV444P14BE, ///< planar YUV 4:4:4,42bpp, (1 Cr & Cb sample per 1x1 Y samples), big-endian
    AV_PIX_FMT_YUV444P14LE, ///< planar YUV 4:4:4,42bpp, (1 Cr & Cb sample per 1x1 Y samples), little-endian
    AV_PIX_FMT_GBRP12BE,    ///< planar GBR 4:4:4 36bpp, big-endian
    AV_PIX_FMT_GBRP12LE,    ///< planar GBR 4:4:4 36bpp, little-endian
    AV_PIX_FMT_GBRP14BE,    ///< planar GBR 4:4:4 42bpp, big-endian
    AV_PIX_FMT_GBRP14LE,    ///< planar GBR 4:4:4 42bpp, little-endian
    AV_PIX_FMT_YUVJ411P,    ///< planar YUV 4:1:1, 12bpp, (1 Cr & Cb sample per 4x1 Y samples) full scale (JPEG), deprecated in favor of AV_PIX_FMT_YUV411P and setting color_range

    AV_PIX_FMT_BAYER_BGGR8,    ///< bayer, BGBG..(odd line), GRGR..(even line), 8-bit samples
    AV_PIX_FMT_BAYER_RGGB8,    ///< bayer, RGRG..(odd line), GBGB..(even line), 8-bit samples
    AV_PIX_FMT_BAYER_GBRG8,    ///< bayer, GBGB..(odd line), RGRG..(even line), 8-bit samples
    AV_PIX_FMT_BAYER_GRBG8,    ///< bayer, GRGR..(odd line), BGBG..(even line), 8-bit samples
    AV_PIX_FMT_BAYER_BGGR16LE, ///< bayer, BGBG..(odd line), GRGR..(even line), 16-bit samples, little-endian
    AV_PIX_FMT_BAYER_BGGR16BE, ///< bayer, BGBG..(odd line), GRGR..(even line), 16-bit samples, big-endian
    AV_PIX_FMT_BAYER_RGGB16LE, ///< bayer, RGRG..(odd line), GBGB..(even line), 16-bit samples, little-endian
    AV_PIX_FMT_BAYER_RGGB16BE, ///< bayer, RGRG..(odd line), GBGB..(even line), 16-bit samples, big-endian
    AV_PIX_FMT_BAYER_GBRG16LE, ///< bayer, GBGB..(odd line), RGRG..(even line), 16-bit samples, little-endian
    AV_PIX_FMT_BAYER_GBRG16BE, ///< bayer, GBGB..(odd line), RGRG..(even line), 16-bit samples, big-endian
    AV_PIX_FMT_BAYER_GRBG16LE, ///< bayer, GRGR..(odd line), BGBG..(even line), 16-bit samples, little-endian
    AV_PIX_FMT_BAYER_GRBG16BE, ///< bayer, GRGR..(odd line), BGBG..(even line), 16-bit samples, big-endian

    AV_PIX_FMT_XVMC,///< XVideo Motion Acceleration via common packet passing

    AV_PIX_FMT_YUV440P10LE, ///< planar YUV 4:4:0,20bpp, (1 Cr & Cb sample per 1x2 Y samples), little-endian
    AV_PIX_FMT_YUV440P10BE, ///< planar YUV 4:4:0,20bpp, (1 Cr & Cb sample per 1x2 Y samples), big-endian
    AV_PIX_FMT_YUV440P12LE, ///< planar YUV 4:4:0,24bpp, (1 Cr & Cb sample per 1x2 Y samples), little-endian
    AV_PIX_FMT_YUV440P12BE, ///< planar YUV 4:4:0,24bpp, (1 Cr & Cb sample per 1x2 Y samples), big-endian
    AV_PIX_FMT_AYUV64LE,    ///< packed AYUV 4:4:4,64bpp (1 Cr & Cb sample per 1x1 Y & A samples), little-endian
    AV_PIX_FMT_AYUV64BE,    ///< packed AYUV 4:4:4,64bpp (1 Cr & Cb sample per 1x1 Y & A samples), big-endian

    AV_PIX_FMT_VIDEOTOOLBOX, ///< hardware decoding through Videotoolbox

    AV_PIX_FMT_P010LE, ///< like NV12, with 10bpp per component, data in the high bits, zeros in the low bits, little-endian
    AV_PIX_FMT_P010BE, ///< like NV12, with 10bpp per component, data in the high bits, zeros in the low bits, big-endian

    AV_PIX_FMT_GBRAP12BE,  ///< planar GBR 4:4:4:4 48bpp, big-endian
    AV_PIX_FMT_GBRAP12LE,  ///< planar GBR 4:4:4:4 48bpp, little-endian

    AV_PIX_FMT_GBRAP10BE,  ///< planar GBR 4:4:4:4 40bpp, big-endian
    AV_PIX_FMT_GBRAP10LE,  ///< planar GBR 4:4:4:4 40bpp, little-endian

    AV_PIX_FMT_MEDIACODEC, ///< hardware decoding through MediaCodec

    AV_PIX_FMT_GRAY12BE,   ///<        Y        , 12bpp, big-endian
    AV_PIX_FMT_GRAY12LE,   ///<        Y        , 12bpp, little-endian
    AV_PIX_FMT_GRAY10BE,   ///<        Y        , 10bpp, big-endian
    AV_PIX_FMT_GRAY10LE,   ///<        Y        , 10bpp, little-endian

    AV_PIX_FMT_P016LE, ///< like NV12, with 16bpp per component, little-endian
    AV_PIX_FMT_P016BE, ///< like NV12, with 16bpp per component, big-endian

    /**
     * Hardware surfaces for Direct3D11.
     *
     * This is preferred over the legacy AV_PIX_FMT_D3D11VA_VLD. The new D3D11
     * hwaccel API and filtering support AV_PIX_FMT_D3D11 only.
     *
     * data[0] contains a ID3D11Texture2D pointer, and data[1] contains the
     * texture array index of the frame as intptr_t if the ID3D11Texture2D is
     * an array texture (or always 0 if it's a normal texture).
     */
    AV_PIX_FMT_D3D11,

    AV_PIX_FMT_GRAY9BE,   ///<        Y        , 9bpp, big-endian
    AV_PIX_FMT_GRAY9LE,   ///<        Y        , 9bpp, little-endian

    AV_PIX_FMT_GBRPF32BE,  ///< IEEE-754 single precision planar GBR 4:4:4,     96bpp, big-endian
    AV_PIX_FMT_GBRPF32LE,  ///< IEEE-754 single precision planar GBR 4:4:4,     96bpp, little-endian
    AV_PIX_FMT_GBRAPF32BE, ///< IEEE-754 single precision planar GBRA 4:4:4:4, 128bpp, big-endian
    AV_PIX_FMT_GBRAPF32LE, ///< IEEE-754 single precision planar GBRA 4:4:4:4, 128bpp, little-endian

    /**
     * DRM-managed buffers exposed through PRIME buffer sharing.
     *
     * data[0] points to an AVDRMFrameDescriptor.
     */
    AV_PIX_FMT_DRM_PRIME,
    /**
     * Hardware surfaces for OpenCL.
     *
     * data[i] contain 2D image objects (typed in C as cl_mem, used
     * in OpenCL as image2d_t) for each plane of the surface.
     */
    AV_PIX_FMT_OPENCL,

    AV_PIX_FMT_GRAY14BE,   ///<        Y        , 14bpp, big-endian
    AV_PIX_FMT_GRAY14LE,   ///<        Y        , 14bpp, little-endian

    AV_PIX_FMT_GRAYF32BE,  ///< IEEE-754 single precision Y, 32bpp, big-endian
    AV_PIX_FMT_GRAYF32LE,  ///< IEEE-754 single precision Y, 32bpp, little-endian

    AV_PIX_FMT_YUVA422P12BE, ///< planar YUV 4:2:2,24bpp, (1 Cr & Cb sample per 2x1 Y samples), 12b alpha, big-endian
    AV_PIX_FMT_YUVA422P12LE, ///< planar YUV 4:2:2,24bpp, (1 Cr & Cb sample per 2x1 Y samples), 12b alpha, little-endian
    AV_PIX_FMT_YUVA444P12BE, ///< planar YUV 4:4:4,36bpp, (1 Cr & Cb sample per 1x1 Y samples), 12b alpha, big-endian
    AV_PIX_FMT_YUVA444P12LE, ///< planar YUV 4:4:4,36bpp, (1 Cr & Cb sample per 1x1 Y samples), 12b alpha, little-endian

    AV_PIX_FMT_NV24,      ///< planar YUV 4:4:4, 24bpp, 1 plane for Y and 1 plane for the UV components, which are interleaved (first byte U and the following byte V)
    AV_PIX_FMT_NV42,      ///< as above, but U and V bytes are swapped

    /**
     * Vulkan hardware images.
     *
     * data[0] points to an AVVkFrame
     */
    AV_PIX_FMT_VULKAN,

    AV_PIX_FMT_Y210BE,    ///< packed YUV 4:2:2 like YUYV422, 20bpp, data in the high bits, big-endian
    AV_PIX_FMT_Y210LE,    ///< packed YUV 4:2:2 like YUYV422, 20bpp, data in the high bits, little-endian

    AV_PIX_FMT_X2RGB10LE, ///< packed RGB 10:10:10, 30bpp, (msb)2X 10R 10G 10B(lsb), little-endian, X=unused/undefined
    AV_PIX_FMT_X2RGB10BE, ///< packed RGB 10:10:10, 30bpp, (msb)2X 10R 10G 10B(lsb), big-endian, X=unused/undefined
    AV_PIX_FMT_NB         ///< number of pixel formats, DO NOT USE THIS if you want to link with shared libav* because the number of formats might differ between versions
};

enum AVColorSpace {
    AVCOL_SPC_RGB         = 0,  ///< order of coefficients is actually GBR, also IEC 61966-2-1 (sRGB)
    AVCOL_SPC_BT709       = 1,  ///< also ITU-R BT1361 / IEC 61966-2-4 xvYCC709 / SMPTE RP177 Annex B
    AVCOL_SPC_UNSPECIFIED = 2,
    AVCOL_SPC_RESERVED    = 3,
    AVCOL_SPC_FCC         = 4,  ///< FCC Title 47 Code of Federal Regulations 73.682 (a)(20)
    AVCOL_SPC_BT470BG     = 5,  ///< also ITU-R BT601-6 625 / ITU-R BT1358 625 / ITU-R BT1700 625 PAL & SECAM / IEC 61966-2-4 xvYCC601
    AVCOL_SPC_SMPTE170M   = 6,  ///< also ITU-R BT601-6 525 / ITU-R BT1358 525 / ITU-R BT1700 NTSC
    AVCOL_SPC_SMPTE240M   = 7,  ///< functionally identical to above
    AVCOL_SPC_YCGCO       = 8,  ///< Used by Dirac / VC-2 and H.264 FRext, see ITU-T SG16
    AVCOL_SPC_YCOCG       = AVCOL_SPC_YCGCO,
    AVCOL_SPC_BT2020_NCL  = 9,  ///< ITU-R BT2020 non-constant luminance system
    AVCOL_SPC_BT2020_CL   = 10, ///< ITU-R BT2020 constant luminance system
    AVCOL_SPC_SMPTE2085   = 11, ///< SMPTE 2085, Y'D'zD'x
    AVCOL_SPC_CHROMA_DERIVED_NCL = 12, ///< Chromaticity-derived non-constant luminance system
    AVCOL_SPC_CHROMA_DERIVED_CL = 13, ///< Chromaticity-derived constant luminance system
    AVCOL_SPC_ICTCP       = 14, ///< ITU-R BT.2100-0, ICtCp
    AVCOL_SPC_NB                ///< Not part of ABI
};
```

### samplefmt

```c++
enum AVSampleFormat {
    AV_SAMPLE_FMT_NONE = -1,
    AV_SAMPLE_FMT_U8,          ///< unsigned 8 bits
    AV_SAMPLE_FMT_S16,         ///< signed 16 bits
    AV_SAMPLE_FMT_S32,         ///< signed 32 bits
    AV_SAMPLE_FMT_FLT,         ///< float
    AV_SAMPLE_FMT_DBL,         ///< double

    AV_SAMPLE_FMT_U8P,         ///< unsigned 8 bits, planar
    AV_SAMPLE_FMT_S16P,        ///< signed 16 bits, planar
    AV_SAMPLE_FMT_S32P,        ///< signed 32 bits, planar
    AV_SAMPLE_FMT_FLTP,        ///< float, planar
    AV_SAMPLE_FMT_DBLP,        ///< double, planar
    AV_SAMPLE_FMT_S64,         ///< signed 64 bits
    AV_SAMPLE_FMT_S64P,        ///< signed 64 bits, planar

    AV_SAMPLE_FMT_NB           ///< Number of sample formats. DO NOT USE if linking dynamically
};

/// @brief Return number of bytes per sample.
int av_get_bytes_per_sample(enum AVSampleFormat sample_fmt);

/// @brief Check if the sample format is planar.
/// @return 1 if the sample format is planar, 0 if it is interleaved
int av_sample_fmt_is_planar(enum AVSampleFormat sample_fmt);

/// @brief Get the required buffer size for the given audio parameters.
int av_samples_get_buffer_size(int *linesize, int nb_channels, int nb_samples,
                               enum AVSampleFormat sample_fmt, int align);

/// @brief Fill plane data pointers and linesize for samples with sample format sample_fmt.
/// @param [out] array to be filled with the pointer for each channel
/// @param [out] calculated linesize, may be NULL
/// @param [in] the pointer to a buffer containing the samples
/// @param [in] the number of channels
/// @param [in] the number of samples in a single channel
/// @param [in] the sample format
/// @param [in] buffer size alignment (0 = default, 1 = no alignment)
/// @return >=0 on success or a negative error code on failure
int av_samples_fill_arrays(uint8_t **audio_data, int *linesize,
                           const uint8_t *buf,
                           int nb_channels, int nb_samples,
                           enum AVSampleFormat sample_fmt, int align);
int av_samples_alloc(uint8_t **audio_data, int *linesize, int nb_channels,
                     int nb_samples, enum AVSampleFormat sample_fmt, int align);

/// @brief Copy samples from src to dst.
int av_samples_copy(uint8_t **dst, uint8_t * const *src, int dst_offset,
                    int src_offset, int nb_samples, int nb_channels,
                    enum AVSampleFormat sample_fmt);

/// @brief Fill an audio buffer with silence.
int av_samples_set_silence(uint8_t **audio_data, int offset, int nb_samples,
                           int nb_channels, enum AVSampleFormat sample_fmt);
```

### time

```c++
/// @brief Get the current time in microseconds.
int64_t av_gettime(void);

/// @brief Get the current time in microseconds since some unspecified starting point.
int64_t av_gettime_relative(void);
```









