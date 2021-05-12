# FFmpeg 4.4 Manual

[TOC]

## Install

```cassandra
x264:
./configure --includedir=/usr/local/include --libdir=/usr/local/lib --enable-shared
make -j10 
make install

ffmpeg:
./configure --enable-gpl --enable-shared --enable-libx264
make -j10
make install
```

## Usage

```c
ffmpeg [options] [[infile options] -i infile]... {[outfile options] outfile}...
```

## Options

### Generic Options

- **-L**: Show license.
- **-h**: Show help.  Possible values of arg are:
  - **long**: Print advanced tool options in addition to the basic tool options.
  - **full**: Print complete list of options, including shared and private options for encoders, decoders, demuxers, muxers, filters, etc.
  - **decoder=decoder_name**: Print detailed information about the decoder named decoder_name. Use the -decoders option to get a list of all decoders.
  - **encoder=encoder_name**: Print detailed information about the encoder named encoder_name. Use the -encoders option to get a list of all encoders.
  - **demuxer=demuxer_name**: Print detailed information about the demuxer named demuxer_name. Use the -formats option to get a list of all demuxers and muxers.
  - **muxer=muxer_name**: Print detailed information about the muxer named muxer_name. Use the -formats option to get a list of all muxers and demuxers.
  - **filter=filter_name**: Print detailed information about the filter named filter_name. Use the -filters option to get a list of all filters.
  - **bsf=bitstream_filter_name**: Print detailed information about the bitstream filter named bitstream_filter_name. Use the -bsfs option to get a list of all bitstream filters.
  - **protocol=protocol_name**: Print detailed information about the protocol named protocol_name. Use the -protocols option to get a list of all protocols.
- **-version**: Show version.
- **-buildconf**: Show the build configuration, one option per line.
- **-formats**: Show available formats (including devices).
- **-demuxers**: Show available demuxers.
- **-muxers**: Show available muxers.
- **-devices**: Show available devices.
- **-codecs**: Show all codecs known to libavcodec.
- **-decoders**: Show available decoders.
- **-encoders**: Show all available encoders.
- **-bsfs**: Show available bitstream filters.
- **-protocols**: Show available protocols.
- **-filters**: Show available libavfilter filters.
- **-pix_fmts**: Show available pixel formats.
- **-sample_fmts**: Show available sample formats.
- **-layouts**: Show channel names and standard channel layouts.
- **-colors**: Show recognized color names.
- **-sources**: Show autodetected sources of the input device.
- **-sinks**: Show autodetected sinks of the output device.
- **-loglevel**: Set logging level and flags used by the library.
  - quiet
  - panic
  - fatal
  - error
  - warning
  - info
  - verbose
  - debug
  - trace

### Main Options

- **-f fmt** (input/output): Force input or output file format.  so this option is not needed in most cases.

- **-i url** (input): input file url

- **-y** (global): Overwrite output files without asking.

- **-n** (global): Do not overwrite output files, and exit immediately if a specified output file already exists.

- **-c codec**: Select an encoder (when used before an output file) or a decoder (when used before an input file) for one or more streams. codec is the name of a decoder/encoder or a special value copy (output only) to indicate that the stream is not to be re-encoded.

  ```cassandra
  ffmpeg -i INPUT -c:v libx264 -c:a copy OUTPUT
  ffmpeg -i INPUT -c copy -c:v:1 libx264 -c:a:137 libvorbis OUTPUT
  ```

- **-t duration** (input/output): 

  - When used as an input option (before `-i`), limit the duration of data read from the input file.
  - When used as an output option (before an output url), stop writing the output after its duration reaches duration.

  ```
  "55": 55 seconds
  "0.2":  0.2 seconds
  "200ms": 200 milliseconds, that’s 0.2s
  "200000us": 200000 microseconds, that’s 0.2s
  "12:03:45: 12 hours, 03 minutes and 45 seconds
  "23.189":  23.189 seconds
  ```

- **-to position** (input/output): Stop writing the output or reading the input at position. position must be a time duration specification

  ```cassandra
  "55": 55 seconds
  "0.2":  0.2 seconds
  "200ms": 200 milliseconds, that’s 0.2s
  "200000us": 200000 microseconds, that’s 0.2s
  "12:03:45: 12 hours, 03 minutes and 45 seconds
  "23.189":  23.189 seconds
  ```

- **-fs limit_size** (output): Set the file size limit, expressed in bytes. No further chunk of bytes is written after the limit is exceeded. The size of the output file is slightly more than the requested file size.

- **-ss position** (input/output): 

  - When used as an input option (before `-i`), seeks in this input file to position. **Note that in most formats it is not possible to seek exactly, so `ffmpeg` will seek to the closest seek point before position**. When transcoding and -accurate_seek is enabled (the default), this extra segment between the seek point and position will be decoded and discarded. When doing stream copy or when -noaccurate_seek is used, it will be preserved.
  - When used as an output option (before an output url), decodes but discards input until the timestamps reach position.

  ```cassandra
  "55": 55 seconds
  "0.2":  0.2 seconds
  "200ms": 200 milliseconds, that’s 0.2s
  "200000us": 200000 microseconds, that’s 0.2s
  "12:03:45: 12 hours, 03 minutes and 45 seconds
  "23.189":  23.189 seconds
  ```

- **-itsoffset offset**: Set the input time offset.

- **-timestamp date** (output): Set the recording timestamp in the container.

  ```cassandra
  [(YYYY-MM-DD|YYYYMMDD)[T|t| ]]((HH:MM:SS[.m...]]])|(HHMMSS[.m...]]]))[Z]
  now
  ```

- **-metadata[:metadata_specifier] key=value** (output,per-metadata): Set a metadata key/value pair.

  ```
  ffmpeg -i in.avi -metadata title="my title" out.flv
  ffmpeg -i INPUT -metadata:s:a:0 language=eng OUTPUT
  ```

- **-frames[:stream_specifier] framecount** (output,per-stream)：Stop writing to the stream after framecount frames.

  - `a`：audio
  - `v`：video

- **-b [:stream_specifier] bitrate**：set bitrate

  - `a`：audio
  - `v`：video

- **-filter[:stream_specifier] filtergraph** (output,per-stream): Create the filtergraph specified by filtergraph and use it to filter the stream.

- **-pre[:stream_specifier] preset_name** (output,per-stream): Specify the preset for matching stream(s).

### Advanded Main Options

- **-map**: Designate one or more input streams as a source for the output file. **Start with 0**

  ```cassandra
  ffmpeg version ... Copyright (c) 2000-2012 the FFmpeg developers
  ...
  Input #0, matroska,webm, from 'input.mkv':
    Duration: 01:39:44.02, start: 0.000000, bitrate: 5793 kb/s
      Stream #0:0(eng): Video: h264 (High), yuv420p, 1920x800, 23.98 fps, 23.98 tbr, 1k tbn, 47.95 tbc (default)
      Stream #0:1(ger): Audio: dts (DTS), 48000 Hz, 5.1(side), s16, 1536 kb/s (default)
      Stream #0:2(eng): Audio: dts (DTS), 48000 Hz, 5.1(side), s16, 1536 kb/s
      Stream #0:3(ger): Subtitle: text (default)
      
  sample 1: 将视频流复制; 将德语音频流编码为MP3（128kbps）和AAC（96kbps）（在输出中创建两个音频流）;将英语音频流删除;将字幕流复制
  ffmpeg -i input.mkv \
      -map 0:0 -map 0:1 -map 0:1 -map 0:3 \
      -c:v copy \
      -c:a:0 libmp3lame -b:a:0 128k \
      -c:a:1 libfaac -b:a:1 96k \
      -c:s copy \
      output.mkv
      
  sample 2: 倒序排列输入流
  ffmpeg -i input.mkv -map 0:3 -map 0:2 -map 0:1 -map 0:0 -c copy output.mkv
  
  sample 3: 我们想从同一个输入文件中仅提取音频流
  ffmpeg -i input.mkv -map 0:1 -map 0:2 -c copy output.mkv
  
  sample 4: 可以使用"-map"命令来创建多路文件输出
  ffmpeg -i input.mkv -map 0:1 -map 0:2 audios_only.mkv -map 0:0 video_only.mkv
  
  sample 5: 将两个视频中的stream拷贝到一个容器中
  ffmpeg -i a.mov -i b.mov -c copy -map 0:2 -map 1:6 out.mov
  ```

- **-benchmark** (global): Show benchmarking information at the end of an encode.

- **-benchmark_all** (global): Show benchmarking information during the encode

- **-timelimit duration**: Exit after ffmpeg has been running for duration seconds in CPU user time.

- **-dump**: Dump each input packet to stderr.

- **-re** (input): Read input at native frame rate. ** It is useful for real-time output (e.g. live streaming).**

- **-vsync paramete**r: Video sync method.

  - `passthrough`: 每一帧从解码器到编码器，时间戳保持不变
  - `cfr`: 如果指定了输出帧率，输入帧会按照需要进行复制（如果输出帧率大于输入帧率）或丢弃（如果输出帧率小于输入帧率）
  - `vfr`: 输入帧从解码器到编码器，时间戳保持不变；如果出现相同时间戳的帧，则丢弃之
  - `drop`: 将所有帧的时间戳清空
  - `auto`: 根据参数自动选择`cfr`或者`vfr`

- **-copytb mode**: Specify how to set the encoder timebase when stream copying. 

  - 1: Use the demuxer timebase.
  - 0: Use the decoder timebase.
  - -1: Try to make the choice automatically

- **-enc_time_base[:stream_specifier] timebase**: Set the encoder timebase. 

  - 0: Assign a default value according to the media type
  - -1: Use the input stream timebase when possible.
  - 1～n: Use the provided number as the timebase    (e.g. 1:24, 1:48000)

- **-bitexact**: Enable bitexact mode for (de)muxer and (de/en)coder

- **-shortest**: Finish encoding when the shortest input stream ends.

- **-bsf[:stream_specifier] bitstream_filters**: Set bitstream filters for matching streams.

  ```cassandra
  ffmpeg -i h264.mp4 -c:v copy -bsf:v h264_mp4toannexb -an out.h264
  ffmpeg -i file.mov -an -vn -bsf:s mov2textsub -c:s copy -f rawvideo sub.txt
  ```

- **-filter_complex filtergraph**: Define a complex filtergraph

- **-filter_complex_threads nb_threads**: Defines how many threads are used to process a filter_complex graph.

- **-thread_queue_size size**: This option sets the maximum number of queued packets when reading from the file or device. **With low latency / high rate live streams, packets may be discarded if they are not read in a timely manner**

- **-discard param**: Allows discarding specific streams or frames from streams.

  - none:  Discard no frame.
  - default: which discards no frames.
  - noref: Discard all non-reference frames.(B)
  - bidir: Discard all bidirectional（双向） frames.
  - nokey: Discard all frames excepts keyframes.
  - all: Discard all frames.

- **-max_muxing_queue_size packets**: 从container中读取video packet，在读到第一个video packet前的packet queue size。默认值足够高，一般

### Video Options

- -r[:stream_specifier] fps: Set frame rate

- -s[:stream_specifier] size: Set frame size. The format is ‘WxH’

- -aspect[:stream_specifier] aspect: 设置视频的宽高比. (eg. "4:3", "16:9")

- -vn: 过滤掉container中的视频

  ```cassandra
  // mp4 -> mp3
  ffmpeg -i input.mp4 -vn output.mp3
  ```

- -pass[:stream_specifier] n

  二次编码。第一次是检测、收集亮度、运动等的相关数据，这样在二次编码的时候就会针对不同的场景来进行动态的压缩编码。

  - 一次编码CBR（固定码率）：保持码率基本维持在平均码率。实现简单，对复杂场景会因码率不足造成马赛克现象，对于简单场景则不能充分利用编码空间
  - 二次编码VBR：比一次编码质量要好一些的。但是编码时间也会增加不少。使用二次编码可以把变化不大的画面转换时码率低一些（如静态画面），而变化大的码率高一些（如打斗动作部分），这样码率是变化的，可以使整部影片的清晰度比较均匀。

  ```cassandra
  // 执行第一次编码，在ffmpeg所在目录会生成ffmpeg2pass-0.log和ffmpeg2pass-0.log.mbtree这两个文件
  ffmpeg -i src.mov -c:v libx264 -preset slow -b:v 2420k -pass 1 -an -f mp4 -y NULL
  
  // 执行第二次编码
  ffmpeg -i src.mov -c:v libx264 -preset slow -b:v 2420k -pass 2 -c:a copy dest6.mp4
  ```

- -passlogfile[:stream_specifier] prefix： Set two-pass log file name prefix

- -vf filtergraph: Create the filtergraph specified by filtergraph and use it to filter the stream.

- -autorotate: Automatically rotate the video according to file metadata

- -autoscale: Automatically scale the video according to the resolution of first frame.

- -pix_fmt[:stream_specifier] format: Set pixel format.
- -sws_flags flags: Set SwScaler flags.
- -ilme: Force interlacing support in encoder(隔行扫描) (MPEG-2 and MPEG-4 only)
- -psnr: Calculate PSNR of compressed frames.
- -vstats: Dump video coding statistics to vstats_HHMMSS.log.
- -vstats_file file: Dump video coding statistics to file.
- -qphist: Show QP histogram
- -copyinkf[:stream_specifier] : When doing stream copy, copy also non-key frames found at the beginning.

### Audio Options

- -aframes number: Set the number of audio frames to output.  **This is an obsolete alias for -frames:a**

- -ar freq: Set the audio sampling frequency
- -aq q: Set the audio quality. **This is an alias for -q:a.**
- -ac channels: Set the number of audio channels.
- -an: 
  - As an input option, blocks all audio streams of a file from being filtered or being automatically selected or mapped for any output.
  - As an output option, disables audio recording i.e. automatic selection or mapping of any audio stream.
- -acodec codec: Set the audio codec. **This is an alias for -codec:a.**
- -sample_fmt sample_fmt: Set the audio sample format. **Use -sample_fmts to get a list of supported sample formats.**
- -af filtergraph: Create the filtergraph specified by filtergraph and use it to filter the stream. **This is an alias for -filter:a**

### Subtitle Options

- -scodec codec: Set the subtitle codec. **This is an alias for -codec:s.**
- -sn:
  - As an input option, blocks all subtitle streams of a file from being filtered or being automatically selected or mapped for any output.
  - As an output option, disables subtitle recording i.e. automatic selection or mapping of any subtitle stream.
- -canvas_size size: Set the size of the canvas used to render subtitles.

## Samples Of Format Conversion

```cassandra
// You can use YUV files as input
ffmpeg -i /tmp/test%d.Y /tmp/out.mpg

// You can input from a raw YUV420P file
ffmpeg -i /tmp/test.yuv /tmp/out.avi

// You can output to a raw YUV420P file
ffmpeg -i mydivx.avi hugefile.yuv

// You can set several input files and output files
ffmpeg -i /tmp/a.wav -s 640x480 -i /tmp/a.yuv /tmp/a.mpg

// You can also do audio and video conversions at the same time
ffmpeg -i /tmp/a.wav -ar 22050 /tmp/a.mp2

// Converts a.wav to a.mp2 at 64 kbits and to b.mp2 at 128 kbits
ffmpeg -i /tmp/a.wav -map 0:a -b:a 64k /tmp/a.mp2 -map 0:a -b:a 128k /tmp/b.mp2
```

## Syntax

### Date

- `[(YYYY-MM-DD|YYYYMMDD)[T|t| ]]((HH:MM:SS[.m...]]])|(HHMMSS[.m...]]]))[Z]`
- `now`

### Time duration

- `55`: 55 seconds
- `0.2`: 0.2 seconds
- `200ms`: 200 milliseconds, 0.2s
- `200000us`: 200000 microseconds, that’s 0.2s
- `12:03:45`: 12 hours, 03 minutes and 45 seconds
- `23.189`: 23.189 seconds

### Video Size

```cassandra
ntsc: 720x480
pal: 720x576
qntsc: 352x240
qpal: 352x288
sntsc: 640x480
spal: 768x576
film: 352x240
ntsc-film: 352x240
sqcif: 128x96
qcif: 176x144
cif: 352x288
4cif: 704x576
16cif: 1408x1152
qqvga: 160x120
qvga: 320x240
vga: 640x480
svga: 800x600
xga: 1024x768
uxga: 1600x1200
qxga: 2048x1536
sxga: 1280x1024
qsxga: 2560x2048
hsxga: 5120x4096
wvga: 852x480
wxga: 1366x768
wsxga: 1600x1024
wuxga: 1920x1200
woxga: 2560x1600
wqsxga: 3200x2048
wquxga: 3840x2400
whsxga: 6400x4096
whuxga: 7680x4800
cga: 320x200
ega: 640x350
hd480: 852x480
hd720: 1280x720
hd1080: 1920x1080
2k: 2048x1080
2kflat: 1998x1080
2kscope: 2048x858
4k: 4096x2160
4kflat: 3996x2160
4kscope: 4096x1716
nhd: 640x360
hqvga: 240x160
wqvga: 400x240
fwqvga: 432x240
hvga: 480x320
qhd: 960x540
2kdci: 2048x1080
4kdci: 4096x2160
uhd2160: 3840x2160
uhd4320: 7680x4320
```

### Color

```cassandra
ffmpeg -colors
```

### Channel Layout

```cassandra
ffmpeg -layouts
```

## Codec Options

- -b:[a|v] integer: Set bitrate in bits/s. Default value is 200K.

- -flags flags: Set generic flags
  - mv4: Use four motion vector by macroblock (mpeg4).
  - qpel: Use 1/4 pel motion compensation.
  - loop: Use loop filter.
  - qscale: Use fixed qscale.
  - pass1: Use internal 2pass ratecontrol in first pass mode.
  - pass2: Use internal 2pass ratecontrol in second pass mode.
  - gray: Only decode/encode grayscale.
  - psnr: Set error[?] variables during encoding.
  - truncated: Input bitstream might be randomly truncated.
  - drop_changed: Don’t output frames whose parameters differ from first decoded frame in stream. Error AVERROR_INPUT_CHANGED is returned when a frame is dropped.
  - ildct: Use interlaced DCT.
  - low_delay: Force low delay.
  - global_header: Place global headers in extradata instead of every keyframe.
  - bitexact: Only write platform-, build- and time-independent data. (except (I)DCT). This ensures that file and data checksums are reproducible and match between platforms. Its primary use is for regression testing.
  - aic: Apply H263 advanced intra coding / mpeg4 ac prediction.
  - ilme: Apply interlaced motion estimation.
  - cgop: Use closed gop.
  - output_corrupt: Output even potentially corrupted frames.
- -time_base rational number: Set codec time base.
- -g integer: Set the group of picture (GOP) size. Default value is 12.
- -frame_size integer: Set audio frame size
- -frame_number integer: Set the frame number
- -qcomp float : Set video quantizer scale compression. Range: [0, 1]
- -qblur float: Set video quantizer scale blur
- -qmin integer: Set min video quantizer scale. Range: [-1, 69]. Default=2
- -qmax integer: Set max video quantizer scale. Range: [-1, 1024]. Default=31
- -bf integer: Set max number of B frames between non-B-frames. **0 means that B-frames are disabled. If a value of -1 is used, it will choose an automatic value depending on the encoder.** Range: [-1, 16]. Default=0
- -b_qfactor float: Set qp factor between P and B frames.
- -b_qoffset float: Set QP offset between P and B frames.
- -maxrate integer: Set max bitrate tolerance
- -minrate integer: Set min bitrate tolerance
- -bufsize integer: Set ratecontrol buffer size
- -i_qfactor float: Set QP factor between P and I frames.
- -i_qoffset float: Set QP offset between P and I frames.
- -mbcmp integer: Set macroblock compare function
- -ildctcmp integer: Set interlaced dct compare function
- -precmp integer: Set pre motion estimation compare function
- -threads integer: Set the number of threads to be used
- -dc integer: Set intra_dc_precision.
- -mblmin integer: Set min macroblock lagrange factor
- -mblmax integer: Set max macroblock lagrange factor
- -keyint_min integer: Set minimum interval between IDR-frames.
- -slices integer: Number of slices, used in parallelized encoding.
- -thread_type flags: Select which multithreading methods to use. Default value is ‘slice+frame’
  - `frame`: Decode more than one frame at once
  - `slice`: Decode more than one part of a single frame at once







