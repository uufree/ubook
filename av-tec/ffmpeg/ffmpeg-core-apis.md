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

### structure

- AVInputFormat：在AVFormatContext中描述input file format的structure

- AVOutputFormat：在AVFormatContext中描述input file format的structure

- AVStream

  ```c++
  typedef struct AVStream {
      int index;    /**< stream index in AVFormatContext */
      
    	/**
       * Format-specific stream ID.
       * decoding: set by libavformat
       * encoding: set by the user, replaced by libavformat if left unset
       */
      int id;
  
      void *priv_data;
  
      /**
       * This is the fundamental unit of time (in seconds) in terms
       * of which frame timestamps are represented.
       *
       * decoding: set by libavformat
       * encoding: May be set by the caller before avformat_write_header() to
       *           provide a hint to the muxer about the desired timebase. In
       *           avformat_write_header(), the muxer will overwrite this field
       *           with the timebase that will actually be used for the timestamps
       *           written into the file (which may or may not be related to the
       *           user-provided one, depending on the format).
       */
      AVRational time_base;
  
      /**
       * Decoding: pts of the first frame of the stream in presentation order, in stream time base.
       * Only set this if you are absolutely 100% sure that the value you set
       * it to really is the pts of the first frame.
       * This may be undefined (AV_NOPTS_VALUE).
       * @note The ASF header does NOT contain a correct start_time the ASF
       * demuxer must NOT set this.
       */
      int64_t start_time;
  
      /**
       * Decoding: duration of the stream, in stream time base.
       * If a source file does not specify a duration, but does specify
       * a bitrate, this value will be estimated from bitrate and file size.
       *
       * Encoding: May be set by the caller before avformat_write_header() to
       * provide a hint to the muxer about the estimated duration.
       */
      int64_t duration;
  
      int64_t nb_frames;                 ///< number of frames in this stream if known or 0
  
      int disposition; /**< AV_DISPOSITION_* bit field */
  
      enum AVDiscard discard; ///< Selects which packets can be discarded at will and do not need to be demuxed.
  
      /**
       * sample aspect ratio(样本纵横比) (0 if unknown)
       * - encoding: Set by user.
       * - decoding: Set by libavformat.
       */
      AVRational sample_aspect_ratio;
  
      AVDictionary *metadata;
  
      /**
       * Average framerate
       *
       * - demuxing: May be set by libavformat when creating the stream or in
       *             avformat_find_stream_info().
       * - muxing: May be set by the caller before avformat_write_header().
       */
      AVRational avg_frame_rate;
  
      /**
       * For streams with AV_DISPOSITION_ATTACHED_PIC disposition, this packet
       * will contain the attached picture.
       *
       * decoding: set by libavformat, must not be modified by the caller.
       * encoding: unused
       */
      AVPacket attached_pic;
  
      /**
       * An array of side data that applies to the whole stream (i.e. the
       * container does not allow it to change between packets).
       *
       * There may be no overlap between the side data in this array and side data
       * in the packets. I.e. a given side data is either exported by the muxer
       * (demuxing) / set by the caller (muxing) in this array, then it never
       * appears in the packets, or the side data is exported / sent through
       * the packets (always in the first packet where the value becomes known or
       * changes), then it does not appear in this array.
       *
       * - demuxing: Set by libavformat when the stream is created.
       * - muxing: May be set by the caller before avformat_write_header().
       *
       * Freed by libavformat in avformat_free_context().
       *
       * @see av_format_inject_global_side_data()
       */
      AVPacketSideData *side_data;
      /**
       * The number of elements in the AVStream.side_data array.
       */
      int            nb_side_data;
  
      /**
       * Flags indicating events happening on the stream, a combination of
       * AVSTREAM_EVENT_FLAG_*.
       *
       * - demuxing: may be set by the demuxer in avformat_open_input(),
       *   avformat_find_stream_info() and av_read_frame(). Flags must be cleared
       *   by the user once the event has been handled.
       * - muxing: may be set by the user after avformat_write_header(). to
       *   indicate a user-triggered event.  The muxer will clear the flags for
       *   events it has handled in av_[interleaved]_write_frame().
       */
      int event_flags;
  /**
   * - demuxing: the demuxer read new metadata from the file and updated
   *     AVStream.metadata accordingly
   * - muxing: the user updated AVStream.metadata and wishes the muxer to write
   *     it into the file
   */
  #define AVSTREAM_EVENT_FLAG_METADATA_UPDATED 0x0001
  /**
   * - demuxing: new packets for this stream were read from the file. This
   *   event is informational only and does not guarantee that new packets
   *   for this stream will necessarily be returned from av_read_frame().
   */
  #define AVSTREAM_EVENT_FLAG_NEW_PACKETS (1 << 1)
  
      /**
       * Real base framerate of the stream.
       * This is the lowest framerate with which all timestamps can be
       * represented(代表) accurately (it is the least common multiple of all
       * framerates in the stream). Note, this value is just a guess!
       * For example, if the time base is 1/90000 and all frames have either
       * approximately 3600 or 1800 timer ticks, then r_frame_rate will be 50/1.
       */
      AVRational r_frame_rate;
  
      /**
       * Codec parameters associated with this stream. Allocated and freed by
       * libavformat in avformat_new_stream() and avformat_free_context()
       * respectively.
       *
       * - demuxing: filled by libavformat on stream creation or in
       *             avformat_find_stream_info()
       * - muxing: filled by the caller before avformat_write_header()
       */
      AVCodecParameters *codecpar;
  
      /*****************************************************************
       * All fields below this line are not part of the public API. They
       * may not be used outside of libavformat and can be changed and
       * removed at will.
       * Internal note: be aware that physically removing these fields
       * will break ABI. Replace removed fields with dummy fields, and
       * add new fields to AVStreamInternal.
       *****************************************************************
       */
  
  #if LIBAVFORMAT_VERSION_MAJOR < 59
      // kept for ABI compatibility only, do not access in any way
      void *unused;
  #endif
  
      int pts_wrap_bits; /**< number of bits in pts (used for wrapping control) */
  
      // Timestamp generation support:
      /**
       * Timestamp corresponding to the last dts sync point.
       *
       * Initialized when AVCodecParserContext.dts_sync_point >= 0 and
       * a DTS is received from the underlying container. Otherwise set to
       * AV_NOPTS_VALUE by default.
       */
      int64_t first_dts;
      int64_t cur_dts;
      int64_t last_IP_pts;
      int last_IP_duration;
  
      /**
       * Number of packets to buffer for codec probing
       */
      int probe_packets;
  
      /**
       * Number of frames that have been demuxed during avformat_find_stream_info()
       */
      int codec_info_nb_frames;
  
      /* av_read_frame() support */
      enum AVStreamParseType need_parsing;
      struct AVCodecParserContext *parser;
  
  #if LIBAVFORMAT_VERSION_MAJOR < 59
      // kept for ABI compatibility only, do not access in any way
      void        *unused7;
      AVProbeData  unused6;
      int64_t      unused5[16+1];
  #endif
      AVIndexEntry *index_entries; /**< Only used if the format does not
                                      support seeking natively. */
      int nb_index_entries;
      unsigned int index_entries_allocated_size;
  
      /**
       * Stream Identifier
       * This is the MPEG-TS stream identifier +1
       * 0 means unknown
       */
      int stream_identifier;
  
  #if LIBAVFORMAT_VERSION_MAJOR < 59
      // kept for ABI compatibility only, do not access in any way
      int unused8;
      int unused9;
      int unused10;
  #endif
  
      /**
       * An opaque field for libavformat internal usage.
       * Must not be accessed in any way by callers.
       */
      AVStreamInternal *internal;
  } AVStream;
  ```

- AVFormatContext

  ```c++
  typedef struct AVFormatContext {
      /**
       * A class for logging and @ref avoptions. Set by avformat_alloc_context().
       * Exports (de)muxer private options if they exist.
       */
      const AVClass *av_class;
  
      /**
       * The input container format.
       *
       * Demuxing only, set by avformat_open_input().
       */
      ff_const59 struct AVInputFormat *iformat;
  
      /**
       * The output container format.
       *
       * Muxing only, must be set by the caller before avformat_write_header().
       */
      ff_const59 struct AVOutputFormat *oformat;
  
      /**
       * Format private data. This is an AVOptions-enabled struct
       * if and only if iformat/oformat.priv_class is not NULL.
       *
       * - muxing: set by avformat_write_header()
       * - demuxing: set by avformat_open_input()
       */
      void *priv_data;
  
      /**
       * I/O context.
       *
       * - demuxing: either set by the user before avformat_open_input() (then
       *             the user must close it manually) or set by avformat_open_input().
       * - muxing: set by the user before avformat_write_header(). The caller must
       *           take care of closing / freeing the IO context.
       *
       * Do NOT set this field if AVFMT_NOFILE flag is set in
       * iformat/oformat.flags. In such a case, the (de)muxer will handle
       * I/O in some other way and this field will be NULL.
       */
      AVIOContext *pb;
  
      /* stream info */
      /**
       * Flags signalling stream properties. A combination of AVFMTCTX_*.
       * Set by libavformat.
       */
      int ctx_flags;
  
      /**
       * Number of elements in AVFormatContext.streams.
       *
       * Set by avformat_new_stream(), must not be modified by any other code.
       */
      unsigned int nb_streams;
      /**
       * A list of all streams in the file. New streams are created with
       * avformat_new_stream().
       *
       * - demuxing: streams are created by libavformat in avformat_open_input().
       *             If AVFMTCTX_NOHEADER is set in ctx_flags, then new streams may also
       *             appear in av_read_frame().
       * - muxing: streams are created by the user before avformat_write_header().
       *
       * Freed by libavformat in avformat_free_context().
       */
      AVStream **streams;
  
  #if FF_API_FORMAT_FILENAME
      /**
       * input or output filename
       *
       * - demuxing: set by avformat_open_input()
       * - muxing: may be set by the caller before avformat_write_header()
       *
       * @deprecated Use url instead.
       */
      attribute_deprecated
      char filename[1024];
  #endif
  
      /**
       * input or output URL. Unlike the old filename field, this field has no
       * length restriction.
       *
       * - demuxing: set by avformat_open_input(), initialized to an empty
       *             string if url parameter was NULL in avformat_open_input().
       * - muxing: may be set by the caller before calling avformat_write_header()
       *           (or avformat_init_output() if that is called first) to a string
       *           which is freeable by av_free(). Set to an empty string if it
       *           was NULL in avformat_init_output().
       *
       * Freed by libavformat in avformat_free_context().
       */
      char *url;
  
      /**
       * Position of the first frame of the component, in
       * AV_TIME_BASE fractional seconds. NEVER set this value directly:
       * It is deduced from the AVStream values.
       *
       * Demuxing only, set by libavformat.
       */
      int64_t start_time;
  
      /**
       * Duration of the stream, in AV_TIME_BASE fractional
       * seconds. Only set this value if you know none of the individual stream
       * durations and also do not set any of them. This is deduced from the
       * AVStream values if not set.
       *
       * Demuxing only, set by libavformat.
       */
      int64_t duration;
  
      /**
       * Total stream bitrate in bit/s, 0 if not
       * available. Never set it directly if the file_size and the
       * duration are known as FFmpeg can compute it automatically.
       */
      int64_t bit_rate;
  
      unsigned int packet_size;
      int max_delay;
  
      /**
       * Flags modifying the (de)muxer behaviour. A combination of AVFMT_FLAG_*.
       * Set by the user before avformat_open_input() / avformat_write_header().
       */
      int flags;
  #define AVFMT_FLAG_GENPTS       0x0001 ///< Generate missing pts even if it requires parsing future frames.
  #define AVFMT_FLAG_IGNIDX       0x0002 ///< Ignore index.
  #define AVFMT_FLAG_NONBLOCK     0x0004 ///< Do not block when reading packets from input.
  #define AVFMT_FLAG_IGNDTS       0x0008 ///< Ignore DTS on frames that contain both DTS & PTS
  #define AVFMT_FLAG_NOFILLIN     0x0010 ///< Do not infer any values from other values, just return what is stored in the container
  #define AVFMT_FLAG_NOPARSE      0x0020 ///< Do not use AVParsers, you also must set AVFMT_FLAG_NOFILLIN as the fillin code works on frames and no parsing -> no frames. Also seeking to frames can not work if parsing to find frame boundaries has been disabled
  #define AVFMT_FLAG_NOBUFFER     0x0040 ///< Do not buffer frames when possible
  #define AVFMT_FLAG_CUSTOM_IO    0x0080 ///< The caller has supplied a custom AVIOContext, don't avio_close() it.
  #define AVFMT_FLAG_DISCARD_CORRUPT  0x0100 ///< Discard frames marked corrupted
  #define AVFMT_FLAG_FLUSH_PACKETS    0x0200 ///< Flush the AVIOContext every packet.
  /**
   * When muxing, try to avoid writing any random/volatile data to the output.
   * This includes any random IDs, real-time timestamps/dates, muxer version, etc.
   *
   * This flag is mainly intended for testing.
   */
  #define AVFMT_FLAG_BITEXACT         0x0400
  #define AVFMT_FLAG_SORT_DTS    0x10000 ///< try to interleave outputted packets by dts (using this flag can slow demuxing down)
  #define AVFMT_FLAG_FAST_SEEK   0x80000 ///< Enable fast, but inaccurate seeks for some formats
  #define AVFMT_FLAG_SHORTEST   0x100000 ///< Stop muxing when the shortest stream stops.
  #define AVFMT_FLAG_AUTO_BSF   0x200000 ///< Add bitstream filters as requested by the muxer
  
      /**
       * Maximum size of the data read from input for determining
       * the input container format.
       * Demuxing only, set by the caller before avformat_open_input().
       */
      int64_t probesize;
  
      /**
       * Maximum duration (in AV_TIME_BASE units) of the data read
       * from input in avformat_find_stream_info().
       * Demuxing only, set by the caller before avformat_find_stream_info().
       * Can be set to 0 to let avformat choose using a heuristic.
       */
      int64_t max_analyze_duration;
  
      const uint8_t *key;
      int keylen;
  
      unsigned int nb_programs;
      AVProgram **programs;
  
      /**
       * Forced video codec_id.
       * Demuxing: Set by user.
       */
      enum AVCodecID video_codec_id;
  
      /**
       * Forced audio codec_id.
       * Demuxing: Set by user.
       */
      enum AVCodecID audio_codec_id;
  
      /**
       * Forced subtitle codec_id.
       * Demuxing: Set by user.
       */
      enum AVCodecID subtitle_codec_id;
  
      /**
       * Maximum amount of memory in bytes to use for the index of each stream.
       * If the index exceeds this size, entries will be discarded as
       * needed to maintain a smaller size. This can lead to slower or less
       * accurate seeking (depends on demuxer).
       * Demuxers for which a full in-memory index is mandatory will ignore
       * this.
       * - muxing: unused
       * - demuxing: set by user
       */
      unsigned int max_index_size;
  
      /**
       * Maximum amount of memory in bytes to use for buffering frames
       * obtained from realtime capture devices.
       */
      unsigned int max_picture_buffer;
  
      /**
       * Number of chapters in AVChapter array.
       * When muxing, chapters are normally written in the file header,
       * so nb_chapters should normally be initialized before write_header
       * is called. Some muxers (e.g. mov and mkv) can also write chapters
       * in the trailer.  To write chapters in the trailer, nb_chapters
       * must be zero when write_header is called and non-zero when
       * write_trailer is called.
       * - muxing: set by user
       * - demuxing: set by libavformat
       */
      unsigned int nb_chapters;
      AVChapter **chapters;
  
      /**
       * Metadata that applies to the whole file.
       *
       * - demuxing: set by libavformat in avformat_open_input()
       * - muxing: may be set by the caller before avformat_write_header()
       *
       * Freed by libavformat in avformat_free_context().
       */
      AVDictionary *metadata;
  
      /**
       * Start time of the stream in real world time, in microseconds
       * since the Unix epoch (00:00 1st January 1970). That is, pts=0 in the
       * stream was captured at this real world time.
       * - muxing: Set by the caller before avformat_write_header(). If set to
       *           either 0 or AV_NOPTS_VALUE, then the current wall-time will
       *           be used.
       * - demuxing: Set by libavformat. AV_NOPTS_VALUE if unknown. Note that
       *             the value may become known after some number of frames
       *             have been received.
       */
      int64_t start_time_realtime;
  
      /**
       * The number of frames used for determining the framerate in
       * avformat_find_stream_info().
       * Demuxing only, set by the caller before avformat_find_stream_info().
       */
      int fps_probe_size;
  
      /**
       * Error recognition; higher values will detect more errors but may
       * misdetect some more or less valid parts as errors.
       * Demuxing only, set by the caller before avformat_open_input().
       */
      int error_recognition;
  
      /**
       * Custom interrupt callbacks for the I/O layer.
       *
       * demuxing: set by the user before avformat_open_input().
       * muxing: set by the user before avformat_write_header()
       * (mainly useful for AVFMT_NOFILE formats). The callback
       * should also be passed to avio_open2() if it's used to
       * open the file.
       */
      AVIOInterruptCB interrupt_callback;
  
      /**
       * Flags to enable debugging.
       */
      int debug;
  #define FF_FDEBUG_TS        0x0001
  
      /**
       * Maximum buffering duration for interleaving.
       *
       * To ensure all the streams are interleaved correctly,
       * av_interleaved_write_frame() will wait until it has at least one packet
       * for each stream before actually writing any packets to the output file.
       * When some streams are "sparse" (i.e. there are large gaps between
       * successive packets), this can result in excessive buffering.
       *
       * This field specifies the maximum difference between the timestamps of the
       * first and the last packet in the muxing queue, above which libavformat
       * will output a packet regardless of whether it has queued a packet for all
       * the streams.
       *
       * Muxing only, set by the caller before avformat_write_header().
       */
      int64_t max_interleave_delta;
  
      /**
       * Allow non-standard and experimental extension
       * @see AVCodecContext.strict_std_compliance
       */
      int strict_std_compliance;
  
      /**
       * Flags indicating events happening on the file, a combination of
       * AVFMT_EVENT_FLAG_*.
       *
       * - demuxing: may be set by the demuxer in avformat_open_input(),
       *   avformat_find_stream_info() and av_read_frame(). Flags must be cleared
       *   by the user once the event has been handled.
       * - muxing: may be set by the user after avformat_write_header() to
       *   indicate a user-triggered event.  The muxer will clear the flags for
       *   events it has handled in av_[interleaved]_write_frame().
       */
      int event_flags;
  /**
   * - demuxing: the demuxer read new metadata from the file and updated
   *   AVFormatContext.metadata accordingly
   * - muxing: the user updated AVFormatContext.metadata and wishes the muxer to
   *   write it into the file
   */
  #define AVFMT_EVENT_FLAG_METADATA_UPDATED 0x0001
  
      /**
       * Maximum number of packets to read while waiting for the first timestamp.
       * Decoding only.
       */
      int max_ts_probe;
  
      /**
       * Avoid negative timestamps during muxing.
       * Any value of the AVFMT_AVOID_NEG_TS_* constants.
       * Note, this only works when using av_interleaved_write_frame. (interleave_packet_per_dts is in use)
       * - muxing: Set by user
       * - demuxing: unused
       */
      int avoid_negative_ts;
  #define AVFMT_AVOID_NEG_TS_AUTO             -1 ///< Enabled when required by target format
  #define AVFMT_AVOID_NEG_TS_MAKE_NON_NEGATIVE 1 ///< Shift timestamps so they are non negative
  #define AVFMT_AVOID_NEG_TS_MAKE_ZERO         2 ///< Shift timestamps so that they start at 0
  
      /**
       * Transport stream id.
       * This will be moved into demuxer private options. Thus no API/ABI compatibility
       */
      int ts_id;
  
      /**
       * Audio preload in microseconds.
       * Note, not all formats support this and unpredictable things may happen if it is used when not supported.
       * - encoding: Set by user
       * - decoding: unused
       */
      int audio_preload;
  
      /**
       * Max chunk time in microseconds.
       * Note, not all formats support this and unpredictable things may happen if it is used when not supported.
       * - encoding: Set by user
       * - decoding: unused
       */
      int max_chunk_duration;
  
      /**
       * Max chunk size in bytes
       * Note, not all formats support this and unpredictable things may happen if it is used when not supported.
       * - encoding: Set by user
       * - decoding: unused
       */
      int max_chunk_size;
  
      /**
       * forces the use of wallclock timestamps as pts/dts of packets
       * This has undefined results in the presence of B frames.
       * - encoding: unused
       * - decoding: Set by user
       */
      int use_wallclock_as_timestamps;
  
      /**
       * avio flags, used to force AVIO_FLAG_DIRECT.
       * - encoding: unused
       * - decoding: Set by user
       */
      int avio_flags;
  
      /**
       * The duration field can be estimated through various ways, and this field can be used
       * to know how the duration was estimated.
       * - encoding: unused
       * - decoding: Read by user
       */
      enum AVDurationEstimationMethod duration_estimation_method;
  
      /**
       * Skip initial bytes when opening stream
       * - encoding: unused
       * - decoding: Set by user
       */
      int64_t skip_initial_bytes;
  
      /**
       * Correct single timestamp overflows
       * - encoding: unused
       * - decoding: Set by user
       */
      unsigned int correct_ts_overflow;
  
      /**
       * Force seeking to any (also non key) frames.
       * - encoding: unused
       * - decoding: Set by user
       */
      int seek2any;
  
      /**
       * Flush the I/O context after each packet.
       * - encoding: Set by user
       * - decoding: unused
       */
      int flush_packets;
  
      /**
       * format probing score.
       * The maximal score is AVPROBE_SCORE_MAX, its set when the demuxer probes
       * the format.
       * - encoding: unused
       * - decoding: set by avformat, read by user
       */
      int probe_score;
  
      /**
       * number of bytes to read maximally to identify format.
       * - encoding: unused
       * - decoding: set by user
       */
      int format_probesize;
  
      /**
       * ',' separated list of allowed decoders.
       * If NULL then all are allowed
       * - encoding: unused
       * - decoding: set by user
       */
      char *codec_whitelist;
  
      /**
       * ',' separated list of allowed demuxers.
       * If NULL then all are allowed
       * - encoding: unused
       * - decoding: set by user
       */
      char *format_whitelist;
  
      /**
       * An opaque field for libavformat internal usage.
       * Must not be accessed in any way by callers.
       */
      AVFormatInternal *internal;
  
      /**
       * IO repositioned flag.
       * This is set by avformat when the underlaying IO context read pointer
       * is repositioned, for example when doing byte based seeking.
       * Demuxers can use the flag to detect such changes.
       */
      int io_repositioned;
  
      /**
       * Forced video codec.
       * This allows forcing a specific decoder, even when there are multiple with
       * the same codec_id.
       * Demuxing: Set by user
       */
      AVCodec *video_codec;
  
      /**
       * Forced audio codec.
       * This allows forcing a specific decoder, even when there are multiple with
       * the same codec_id.
       * Demuxing: Set by user
       */
      AVCodec *audio_codec;
  
      /**
       * Forced subtitle codec.
       * This allows forcing a specific decoder, even when there are multiple with
       * the same codec_id.
       * Demuxing: Set by user
       */
      AVCodec *subtitle_codec;
  
      /**
       * Forced data codec.
       * This allows forcing a specific decoder, even when there are multiple with
       * the same codec_id.
       * Demuxing: Set by user
       */
      AVCodec *data_codec;
  
      /**
       * Number of bytes to be written as padding in a metadata header.
       * Demuxing: Unused.
       * Muxing: Set by user via av_format_set_metadata_header_padding.
       */
      int metadata_header_padding;
  
      /**
       * User data.
       * This is a place for some private data of the user.
       */
      void *opaque;
  
      /**
       * Callback used by devices to communicate with application.
       */
      av_format_control_message control_message_cb;
  
      /**
       * Output timestamp offset, in microseconds.
       * Muxing: set by user
       */
      int64_t output_ts_offset;
  
      /**
       * dump format separator.
       * can be ", " or "\n      " or anything else
       * - muxing: Set by user.
       * - demuxing: Set by user.
       */
      uint8_t *dump_separator;
  
      /**
       * Forced Data codec_id.
       * Demuxing: Set by user.
       */
      enum AVCodecID data_codec_id;
  
      /**
       * ',' separated list of allowed protocols.
       * - encoding: unused
       * - decoding: set by user
       */
      char *protocol_whitelist;
  
      /**
       * A callback for opening new IO streams.
       *
       * Whenever a muxer or a demuxer needs to open an IO stream (typically from
       * avformat_open_input() for demuxers, but for certain formats can happen at
       * other times as well), it will call this callback to obtain an IO context.
       *
       * @param s the format context
       * @param pb on success, the newly opened IO context should be returned here
       * @param url the url to open
       * @param flags a combination of AVIO_FLAG_*
       * @param options a dictionary of additional options, with the same
       *                semantics as in avio_open2()
       * @return 0 on success, a negative AVERROR code on failure
       *
       * @note Certain muxers and demuxers do nesting, i.e. they open one or more
       * additional internal format contexts. Thus the AVFormatContext pointer
       * passed to this callback may be different from the one facing the caller.
       * It will, however, have the same 'opaque' field.
       */
      int (*io_open)(struct AVFormatContext *s, AVIOContext **pb, const char *url,
                     int flags, AVDictionary **options);
  
      /**
       * A callback for closing the streams opened with AVFormatContext.io_open().
       */
      void (*io_close)(struct AVFormatContext *s, AVIOContext *pb);
  
      /**
       * ',' separated list of disallowed protocols.
       * - encoding: unused
       * - decoding: set by user
       */
      char *protocol_blacklist;
  
      /**
       * The maximum number of streams.
       * - encoding: unused
       * - decoding: set by user
       */
      int max_streams;
  
      /**
       * Skip duration calcuation in estimate_timings_from_pts.
       * - encoding: unused
       * - decoding: set by user
       */
      int skip_estimate_duration_from_pts;
  
      /**
       * Maximum number of packets that can be probed
       * - encoding: unused
       * - decoding: set by user
       */
      int max_probe_packets;
  } AVFormatContext;
  ```

## AVCodec

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
```



## AVFilter

