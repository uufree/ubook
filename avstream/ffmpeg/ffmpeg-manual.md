# FFmpeg 4.4 Manual

[TOC]

## Install

```cassandra
x264:
git clone https://code.videolan.org/videolan/x264.git
cd x264
./configure --includedir=/usr/local/include --libdir=/usr/local/lib --enable-shared
make -j10 
make install

x265:
hg clone http://hg.videolan.org/x265
cd x265/build/linux
./make-Makefiles.bash
make -j10
make install

mp3:
wget https://jaist.dl.sourceforge.net/project/lame/lame/3.99/lame-3.99.5.tar.gz
tar -zxvf lame-3.99.5.tar.gz
cd lame-3.99.5
./configure --enable-shared
make -j10
make install

vpx:

av1:


ffmpeg:
./configure --enable-gpl --enable-shared --enable-libx264 --enable-libx265 --enable-libmp3lame
make -j10
make install
```

## Usage

```c
ffmpeg [options] [[infile options] -i infile]... {[outfile options] outfile}...
```

## Options

### Print Options

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

### Live Streaming Options

- **-re**: 以本地帧频读数据，主要用于模拟捕获设备。 **It is useful for real-time output (e.g. live streaming).**
- **-thread_queue_size size**: This option sets the maximum number of queued packets when reading from the file or device. **With low latency / high rate live streams, packets may be discarded if they are not read in a timely manner**

### Main Options

- **-f fmt** (input/output): Force input or output file format.  so this option is not needed in most cases. see: `ffmpeg -formats`

  ```cassandra
  ffmpeg -i audio.mp4 -f mp4 test.mp4
  ```

- **-i url** (input): input file url

- **-y** (global): Overwrite output files without asking.

- **-n** (global): Do not overwrite output files, and exit immediately if a specified output file already exists.

- **-c[:stream_specifier] codec**: 

  ```cassandra
  ffmpeg -i INPUT -c:v libx264 -c:a copy OUTPUT
  ffmpeg -i INPUT -c copy -c:v:1 libx264 -c:a:137 libvorbis OUTPUT
  ```

- **-t duration** (input/output): 指定截取时间段的长度

- **-to position** (input/output): 指定截取终止时间点

- **-ss position** (input/output): 指定截取起始位置。**Note that in most formats it is not possible to seek exactly, so `ffmpeg` will seek to the closest seek point before position**. 

  ```cassandra
  "55": 55 seconds
  "0.2":  0.2 seconds
  "200ms": 200 milliseconds, that’s 0.2s
  "200000us": 200000 microseconds, that’s 0.2s
  "12:03:45: 12 hours, 03 minutes and 45 seconds
  "23.189":  23.189 seconds
  
  // 从第10s开始，截取20s的视频片段. 
  ffmpeg -i 20200904-a.mp4 -ss 10 -t 20 -c:v copy -c:a copy -y cut.mp4
  **-thread_queue_size size**: This option sets the maximum number of queued packets when reading from the file or device. **With low latency / high rate live streams, packets may be discarded if they are not read in a timely manner**
  // 从第10s开始，截取终止点为30s的视频片段
  ffmpeg -i 20200904-a.mp4 -ss 10 -to 30 -c:v copy -c:a copy -y cut.mp4
  
  // 如果视频比较长，可以选择先定位，再解码（这样不太准，建议片段前后预留3s）
  ffmpeg -ss 01:00:00 -i 20200904-a.mp4  -t 20 -c:v copy -c:a copy -y cut.mp4
  ```

- **-metadata[:metadata_specifier] key=value** (output,per-metadata): Set a metadata key/value pair.

  ```cassandra
  ffmpeg -i in.avi -c copy -metadata title="my title" out.flv
  ```

- **-frames[:stream_specifier] framecount** (output,per-stream)：Stop writing to the stream after framecount frames.

  ```cassandra
  // 获取视频帧数
  ffprobe -v error -count_frames -select_streams v:0 -show_entries stream=nb_read_frames -of default=nokey=1:noprint_wrappers=1 cut.mp4
  
  // 截取指定帧数的视频
  ffmpeg -i 20200904-a.mp4 -c copy -frames:v 1000 -frames:a cut.mp4
  ```

- **-b [:stream_specifier] bitrate**：set bitrate

  ```cassandra
  ffmpeg -i 20200904-a.mp4 -c:v libx264 -c:a libmp3lame -c:s copy -b:v 500k  -b:a 80k -y convert.mp4
  ```

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

  ```cassandra
  ffmpeg -i 20200904-a.mp4 -c copy -benchmark -y cut.mp4
  ffmpeg -i 20200904-a.mp4 -c copy -benchmark_all -y cut.mp4
  ```

- **-dump**: Dump each input packet to stderr.

  ```cassandra
  ffmpeg -i 20200904-a.mp4 -c copy -benchmark_all -dump -y cut.mp4 
  
  // output
  stream #1:
    keyframe=1
    duration=0.023
    dts=313.585  pts=313.585
    size=365
  stream #1:
    keyframe=1
    duration=0.023
    dts=313.609  pts=313.609
    size=364
  ```

- **-vsync paramete**r: Video sync method.

  - `passthrough`: 每一帧从解码器到编码器，时间戳保持不变
  - `cfr`: 如果指定了输出帧率，输入帧会按照需要进行复制（如果输出帧率大于输入帧率）或丢弃（如果输出帧率小于输入帧率）
  - `vfr`: 输入帧从解码器到编码器，时间戳保持不变；如果出现相同时间戳的帧，则丢弃之
  - `drop`: 将所有帧的时间戳清空
  - `auto`: 根据参数自动选择`cfr`或者`vfr`

  ```cassandra
  ffmpeg -i 20200904-a.mp4 -an -c copy -vsync cfr -y cut.mp4
  ```

- **-bsf[:stream_specifier] bitstream_filters**: Set bitstream filters for matching streams.

  ```cassandra
  // 去除码流中多余的pps和sps
  ffmpeg -i h264.mp4 -c:v copy -bsf:v h264_mp4toannexb -an out.h264
  
  // 处理字幕流
  ffmpeg -i file.mov -an -vn -bsf:s mov2textsub -c:s copy -f rawvideo sub.txt
  ```

- **-discard param**: 从流中丢弃特定的帧

  - none:  不丢弃任何帧
  - default: 不求任何帧
  - noref: Discard all non-reference frames.(B)
  - bidir: Discard all bidirectional(双向) frames.(P, B)
  - nokey: Discard all frames excepts keyframes.(IDR, P,B)
  - all: Discard all frames.

- **-psnr**: Calculate PSNR of compressed frames.

  ```cassandra
  ffmpeg -i pass1.mp4 -an -c:v libx264 -y -psnr -f null -
  ```

- **-vstats**: Dump video coding statistics to vstats_HHMMSS.log.

- **-vstats_file file**: Dump video coding statistics to file.

  ```cassandra
  ffmpeg -i pass1.mp4 -an -c:v libx264 -y -vstats cut.mp4
  ```

- **-qphist**: Show QP histogram

  ```cassandra
  ffmpeg -i pass1.mp4 -an -c:v libx264 -y -qphist cut.mp4
  ```

- **-copyinkf[:stream_specifier]** : When doing stream copy, copy also non-key frames found **at the beginning.**

  ```cassandra
  ffmpeg -i pass1.mp4 -c copy -copyinkf -y cut.mp4
  ```
  
- **-max_muxing_queue_size packets**: 从container中读取video packet，在读到第一个video packet前的packet queue size。默认值足够高，一般无需设置

- **（不会用）-filter_complex filtergraph**: Define a complex filtergraph

- **（不会用）-filter_complex_threads nb_threads**: Defines how many threads are used to process a filter_complex graph.

- **（不会用）-filter[:stream_specifier] filtergraph** (output,per-stream): Create the filtergraph specified by filtergraph and use it to filter the stream.

- **（不会用）-pre[:stream_specifier] preset_name** (output,per-stream): Specify the preset for matching stream(s).

- **（不常用）-timestamp date** (output): Set the recording timestamp in the container.

  ```cassandra
  [(YYYY-MM-DD|YYYYMMDD)[T|t| ]]((HH:MM:SS[.m...]]])|(HHMMSS[.m...]]]))[Z]
  now
  ```

- **（不常用）-fs limit_size**: set the limit file size in bytes

  ```cassandra
  ffmpeg -i 20200904-a.mp4 -c copy -y -fs 1000 cut.mp4
  ```

- **（不常用）-enc_time_base[:stream_specifier] timebase**: Set the encoder timebase. 

  - 0: Assign a default value according to the media type
  - -1: Use the input stream timebase when possible.
  - 1～n: Use the provided number as the timebase    (e.g. 1:24, 1:48000)

- **（不常用）-bitexact**: Enable bitexact mode for (de)muxer and (de/en)coder

- **（不常用）-shortest**: Finish encoding when the shortest input stream ends.

- **（不常用）-copytb mode**: Specify how to set the encoder timebase when stream copying. 

  - 1: Use the demuxer timebase.
  - 0: Use the decoder timebase.
  - -1: Try to make the choice automatically

- **（不常用）-timelimit duration**: Exit after ffmpeg has been running for duration seconds in CPU user time.

### Video Options

- **-r[:stream_specifier] fps**: Set frame rate

  ```cassandra
  // 设置帧率
  ffmpeg -i 20200904-a.mp4 -c libx264 -an -r 10 -y cut.mp4
  
  // 设置视频和音频的帧率
  ffmpeg -i 20200904-a.mp4 -c:v libx264 -c:a aac -r:v 10 -r:a 38 -y cut.mp4
  ```

- **-s[:stream_specifier] size**: Set frame size. The format is ‘WxH’

  ```cassandra
  ffmpeg -i 20200904-a.mp4 -c:v libx264 -s 500x500 -y cut.mp4
  ```

- **-aspect[:stream_specifier] aspect**: 设置视频的宽高比. (eg. "4:3", "16:9")

  ```cassandra
  ffmpeg -i 20200904-a.mp4 -c:v libx264 -s 500x500 -aspect:v 4:3 -y cut.mp4
  ```

- **-vn**: 过滤掉container中的视频

  ```cassandra
  // mp4 -> mp3
  ffmpeg -i input.mp4 -vn output.mp3
  ```

- **-pass[:stream_specifier] n**

  二次编码。第一次是检测、收集亮度、运动等的相关数据，这样在二次编码的时候就会针对不同的场景来进行动态的压缩编码。

  - 一次编码CBR（固定码率）：保持码率基本维持在平均码率。实现简单，对复杂场景会因码率不足造成马赛克现象，对于简单场景则不能充分利用编码空间
  - 二次编码VBR：比一次编码质量要好一些的。但是编码时间也会增加不少。使用二次编码可以把变化不大的画面转换时码率低一些（如静态画面），而变化大的码率高一些（如打斗动作部分），这样码率是变化的，可以使整部影片的清晰度比较均匀。

  ```cassandra
  // 执行第一次编码，会在当前目录会生成ffmpeg2pass-0.log和ffmpeg2pass-0.log.mbtree这两个文件
  ffmpeg -i 20200904-a.mp4 -c:v libx264 -c:a copy -c:s copy -b:v 2420k -pass 1 -y pass1.mp4
  
  // 执行第二次编码
  ffmpeg -i 20200904-a.mp4 -c:v libx264 -c:a copy -c:s copy -b:v 2420k -pass 2 -y pass2.mp4
  
  // 还可以执行第三次编码，不清楚这一次的优化点是啥
  ffmpeg -i 20200904-a.mp4 -c:v libx264 -c:a copy -c:s copy -b:v 2420k -pass 3 -y pass3.mp4
  ```

- **-pix_fmt[:stream_specifier] format**: Set pixel format. see: `ffmpeg -pix_fmts`

  ```cassandra
  ffmpeg -i 20200904-a.mp4 -c:v libx264 -c:a copy -pix_fmt:v yuv444p -y cut.mp4
  ```

- **（不会用）-vf filtergraph**: Create the filtergraph specified by filtergraph and use it to filter the stream.

- **（不会用）-autorotate**: Automatically rotate the video according to file metadata

- **（不会用）-autoscale**: Automatically scale the video according to the resolution of first frame.

- **（不会用）-sws_flags flags**: Set SwScaler flags.

### Audio Options

- **-ar freq**: Set the audio sampling frequency

  ```cassandra
  ffmpeg -i 20200904-a.mp4 -c:v copy -c:a aac -ar 22050 -y cut.mp4
  ```

- **-ac channels**: Set the number of audio channels.

  ```cassandra
  // 将双声道音频（L+R）转为3声道音频（L+R+C）
  ffmpeg -i 20200904-a.mp4 -c:v copy -c:a aac -ac 3 -y cut.mp4
  ```

- **-an**: 去除容器中的音频流

- **-sample_fmt sample_fmt**: Set the audio sample format. see: `ffmpeg -sample_fmts`

  ```cassandra
  ffmpeg -i 20200904-a.mp4 -c:v copy -c:a aac -sample_fmt fltp -y cut.mp4
  ```

- （**不会用）-af filtergraph**: Create the filtergraph specified by filtergraph and use it to filter the stream. **This is an alias for -filter:a**

### Subtitle Options

- **-sn**: 删除容器中的字幕流
- **-canvas_size size**: Set the size of the canvas used to render subtitles.

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

## Codec Options

- **-flags flags**: Set generic flags.  (猜测：使用的可能不是很频繁)
  
  - **mv4**: 每个宏块使用4个运动向量描述 (mpeg4).
  - **qpel**: 使用1/4像素的运动补偿
  - **loop**: 使用环路滤波
  - **qscale**: Use fixed qscale.
  - **pass1**: Use internal 2pass ratecontrol in first pass mode.
  - **pass2**: Use internal 2pass ratecontrol in second pass mode.
  - **gray**: Only decode/encode grayscale.
  - **psnr**: Set error[?] variables during encoding.
  - **truncated**: 输入的比特流可能被随机截断
  - **drop_changed**: Don’t output frames whose parameters differ from first decoded frame in stream. Error AVERROR_INPUT_CHANGED is returned when a frame is dropped.
  - **ildct**: 使用隔行DCT变换
  - **low_delay**: 强制低延迟
  - **global_header**: Place global headers in extradata instead of every keyframe.
  - **bitexact**: Only write platform-, build- and time-independent data. (except (I)DCT). This ensures that file and data checksums are reproducible and match between platforms. Its primary use is for regression testing.
  - **aic**: Apply H263 advanced intra coding / mpeg4 ac prediction.
  - **ilme**: Apply interlaced motion estimation.
  - **cgop**: Use closed gop.
  - **output_corrupt**: Output even potentially corrupted frames.
  
  ```cassandra
  ffmpeg -i 20200904-a.mp4 -c:v libx264 -c:a copy -y -flags mv4 -flags qpel cut.mp4
  ```
  
- **-g integer**: Set the group of picture (GOP) size. Default value is 12.

  ```cassandra
  // 设置一个gop中帧的数量
  ffmpeg -i 20200904-a.mp4 -c:v libx264 -c:a copy -g 8 -y cut.mp4
  ```

- **-qcomp float** : video quantizer scale compression (VBR).. Range: [0, 1]. Default: 0.5. 

  ```cassandra
  // qcomp越高，码率越大，效果质量越好
  ffmpeg -i 20200904-a.mp4 -c:v libx264 -qcomp 1 -c:a copy  -y cut.mp4
  ```

- **-qblur float**: video quantizer scale blur (VBR). Range: [-1, FLOAT_MAX]. Default: 0.5

- **-qmin integer**: minimum video quantizer scale (VBR). Range: [-1, 69]. Default=2

- **-qmax integer**: maximum video quantizer scale (VBR). Range: [-1, 1024]. Default=31

- **-bf integer**: Set max number of B frames between non-B-frames. **0 means that B-frames are disabled. If a value of -1 is used, it will choose an automatic value depending on the encoder.** Range: [-1, 16]. Default=0

- **-b_qfactor float**: Set qp factor between P and B frames.

- **-b_qoffset float**: Set QP offset between P and B frames.

- **-maxrate integer**: Set max bitrate tolerance

- **-minrate integer**: Set min bitrate tolerance

- **-bufsize integer**: Set ratecontrol buffer size

- **-i_qfactor float**: Set QP factor between P and I frames.

- **-i_qoffset float**: Set QP offset between P and I frames.

- **-mbcmp integer**: Set macroblock compare function

- **-ildctcmp integer**: Set interlaced dct compare function

- **-precmp integer**: Set pre motion estimation compare function

- **-threads integer**: Set the number of threads to be used

  ```cassandra
  // 限制编解码过程中使用的线程数量
  ffmpeg -i 20200904-a.mp4 -c:v libx264 -threads 4 -c:a copy  -y cut.mp4
  ```

- **-dc integer**: Set intra_dc_precision.

- **-mblmin integer**: Set min macroblock lagrange factor

- **-mblmax integer**: Set max macroblock lagrange factor

- **-keyint_min integer**: Set minimum interval between IDR-frames.

- **-slices integer**: Number of slices, used in parallelized encoding. 

- **-thread_type flags**: Select which multithreading methods to use. Default value is ‘slice+frame’
  
  - `frame`: Decode more than one frame at once
  - `slice`: Decode more than one part of a single frame at once

## Audio Encoders

### aac

- **-aac_coder method**: Set AAC encoder coding method

  - `twoloop`: Two loop searching (TLS) method. (质量高)
  - `anmr`: Average noise to mask ratio (ANMR) trellis-based solution.(不推荐使用)
  - `fast`: Constant quantizer method.  **This is the default choice for a coder**. 

  ```cassandra
  ffmpeg -i 20200904-a.mp4 -vn -c:a aac -aac_coder twoloop -y cut.mp4
  ffmpeg -i 20200904-a.mp4 -vn -c:a aac -aac_coder fast -y cut.mp4
  ```

- **-aac_ms**: Sets mid/side coding mode. Default: `auto`

- **-aac_is**: Sets intensity stereo coding tool usage. Default: `enable`

- **-aac_pns**: Uses perceptual noise substitution to replace low entropy high frequency bands with imperceptible white noise during the decoding process. Default: `enable`

- **-aac_tns**: Enables the use of a multitap FIR filter which spans through the high frequency bands to hide quantization noise during the encoding process and is reverted by the decoder. Default: `enable`

- **-aac_ltp**: Enables the use of the long term prediction extension which increases coding efficiency in very low bandwidth situations such as encoding of voice or solo piano music by extending constant harmonic peaks in bands throughout frames. 

- **-aac_pred**: Enables the use of a more traditional style of prediction where the spectral coefficients transmitted are replaced by the difference of the current coefficients minus the previous "predicted" coefficients.

- **-profile**: Sets the encoding profile

  - `aac_low`: Default
  - `mpeg2_aac_low`
  - `aac_ltp`: Long term prediction profile
  - `aac_main`: Main-type prediction profile

  ```cassandra
  ffmpeg -i 20200904-a.mp4 -vn -c:a aac -profile aac_low -y cut.mp4
  ffmpeg -i 20200904-a.mp4 -vn -c:a aac -profile aac_main -y cut.mp4
  ```

### libmp3lame

https://www.ffmpeg.org/ffmpeg-all.html#libmp3lame-1

## Video Encoders

### libvpx

https://www.ffmpeg.org/ffmpeg-all.html#toc-libvpx

### libx264, libx264rgb

https://www.ffmpeg.org/ffmpeg-all.html#toc-libx264_002c-libx264rgb

### libx265

https://www.ffmpeg.org/ffmpeg-all.html#toc-libx265

## Subtitle Encoders

### dvdsub

https://www.ffmpeg.org/ffmpeg-all.html#toc-dvdsub-1

## Bitstream Filters

The option `-bsfs` of the ff* tools will display the list of all the supported bitstream filters included in your build.

```cassandra
// show all bfs
ffmpeg -bsfs

// command template
ffmpeg -i INPUT -c:v copy -bsf:v filter1[=opt1=str1:opt2=str2][,filter2] OUTPUT
```

- **aac_adtstoasc**

  Convert MPEG-2/4 AAC ADTS to an MPEG-4 Audio Specific Configuration bitstream.

  This filter creates an MPEG-4 AudioSpecificConfig from an MPEG-2/4 ADTS header and removes the ADTS header.

  This filter is required for example when copying an AAC stream from a raw ADTS AAC or an MPEG-TS container to MP4A-LATM, to an FLV file, or to MOV/MP4 files and related formats such as 3GP or M4A. Please note that it is auto-inserted for MP4A-LATM and MOV/MP4 and related formats.

- **av1_metadata**

- **chomp**: Remove zero padding at the end of a packet.

- **dca_core**: Extract the core from a DCA/DTS stream, dropping extensions such as DTS-HD.

- **dump_extra**: Add extradata to the beginning of the filtered packets

  - keyframe: add extradata to all key packets
  - all: add extradata to all packets

  ```cassandra
  ffmpeg -i INPUT -map 0 -flags:v +global_header -c:v libx264 -bsf:v dump_extra out.ts
  ```

- **eac3_core**

- **extract_extradata**: This bitstream filter detects the in-band headers and makes them available as extradata.

- **filter_units**

- **hapqa_extract**

- **h264_metadata**: Modify metadata embedded in an H.264 stream.

- **hevc_metadata**:  Modify metadata embedded in an H.265 stream.

  - **aud**
  - **sample_aspect_ratio**
  - **overscan_appropriate_flag**
  - **video_format**
  - **video_full_range_flag**
  - **colour_primaries**
  - **transfer_characteristics**
  - **matrix_coefficients**
  - **chroma_sample_loc_type**
  - **fixed_frame_rate_flag**
  - **tick_rate**
  - **crop_left**
  - **crop_right**
  - **crop_top**
  - **crop_bottom**
  - **sei_user_data**
  - **delete_filler**
  - **level**

- **h264_mp4toannexb**

- **hevc_metadata**

  h264有两种封装格式，字节流AnnexB和AVCC格式

  - **AnnexB格式**，用于实时播放，对应ts容器。AnnexB格式每个NALU都包含起始码，且通常会周期性的在关键帧之前重复SPS和PPS。所以解码器可以从视频流随机点开始进行解码，实时的流格式。
  - **AVCC格式**，用于存储，对用mp4容器。AVCC格式不使用起始码作为NALU的分界，这种格式在每个NALU前都加上一个大端格式的前缀（1、2、4字节，代表NALU长度）。所以在解析AVCC格式的时候需要将指定的前缀字节数的值保存在一个头部对象中，这个都通常称为extradata或者sequence header。同时，SPS和PPS数据也需要保存在extradata或者叫’sequence header’中。

  ```cassandra
  // mp4转ts
  ffmpeg -i INPUT.mp4 -codec copy -bsf:v h264_mp4toannexb OUTPUT.ts
  ```

- **h264_redundant_pps**

- **imxdump**

- **mjpeg2jpeg**: Convert MJPEG/AVI1 packets to full JPEG/JFIF packets.

  ```cassandra
  ffmpeg -i mjpeg-movie.avi -c:v copy -bsf:v mjpeg2jpeg frame_%d.jpg
  ```

- **mjpegadump**

- **mov2textsub**: Extract a representable text file from MOV subtitles, stripping the metadata header from each subtitle packet.

- **mp3decomp**

- **mpeg2_metadata**: Modify metadata embedded in an MPEG-2 stream.

- **mpeg4_unpack_bframes**: Unpack DivX-style packed B-frames.

  ```cassandra
  ffmpeg -i INPUT.avi -codec copy -bsf:v mpeg4_unpack_bframes OUTPUT.avi
  ```

- **noise**

- **null**

- **pcm_rechunk**

- **prores_metadata**

- **remove_extra**: Remove extradata from packets.

  - freq
    - `k`： Remove extradata from non-keyframes only
    - `keyframe`: Remove extradata from keyframes only.
    - `all`: Remove extradata from all frames.

- **setts**

- **text2movsub**

- **trace_headers**

- **truehd_core**

- **vp9_metadata**: Modify metadata embedded in a VP9 stream.

- **vp9_superframe**: Merge VP9 invisible (alt-ref) frames back into VP9 superframes. 

- **vp9_superframe_split**: Split VP9 superframes into single frames.

- **vp9_raw_reorder**: Given a VP9 stream with correct timestamps but possibly out of order, insert additional show-existing-frame packets to correct the ordering.

## Format Options

### Demuxer

#### HLS

HLS demuxer, Apple HTTP Live Streaming demuxer.

https://www.ffmpeg.org/ffmpeg-all.html#toc-hls-1

#### MP4

https://www.ffmpeg.org/ffmpeg-all.html#toc-mov_002fmp4_002f3gp

#### MPEG-TS

https://www.ffmpeg.org/ffmpeg-all.html#toc-mpegts

### Muxer

#### AVI

https://www.ffmpeg.org/ffmpeg-all.html#toc-avi-1

#### FLV

https://www.ffmpeg.org/ffmpeg-all.html#toc-flv

#### HLS

https://www.ffmpeg.org/ffmpeg-all.html#toc-hls-2

#### MP4

https://www.ffmpeg.org/ffmpeg-all.html#toc-mov_002c-mp4_002c-ismv

#### MP3

https://www.ffmpeg.org/ffmpeg-all.html#toc-mp3

#### MPEG-TS

https://www.ffmpeg.org/ffmpeg-all.html#toc-mpegts-1

### Special 

#### segment

- -segment_format format: Override the inner container format, by default it is guessed by the filename extension.
- -segment_list name: Generate also a listfile named name. If not specified no listfile is generated.
- -segment_list_flags flags: Generate also a listfile named name. If not specified no listfile is generated.
  - `cache`: Allow caching (only affects M3U8 list files)
  - `live`: Allow live-friendly file generation
- -segment_list_entry_prefix prefix: Prepend prefix to each entry. Useful to generate absolute paths. By default no prefix is applied.
- -segment_list_type type: Select the listing format.
  - `flat`: Generate a flat list for the created segments, one segment per line.
  - `m3u8`: Generate an extended M3U8 file
- -segment_time time: Set segment duration to time, the value must be a duration specification.
- -segment_times times: Specify a list of split points
- -segment_frames frames: Specify a list of split video frame numbers.
- -segment_start_number number: Set the sequence number of the first segment.

```cassandra
ffmpeg -i in.mkv -codec hevc -flags +cgop -g 60 -map 0 -f segment -segment_list out.list out%03d.nut

ffmpeg -i in.mkv -codec copy -map 0 -f segment -segment_list out.csv -segment_times 1,2,3,5,8,13,21 out%03d.nut

ffmpeg -i in.mkv -codec copy -map 0 -f segment -segment_list out.csv -segment_frames 100,200,300,500,800 out%03d.nut
```

#### tee

The tee muxer can be used to write the same data to several outputs, such as files or streams. 

```cassandra
ffmpeg -i ... -c:v libx264 -c:a mp2 -f tee -map 0:v -map 0:a "archive-20121107.mkv|[f=mpegts]udp://10.0.1.255:1234/"
```

## Protocols

### ftp

https://www.ffmpeg.org/ffmpeg-all.html#ftp

### hls

https://www.ffmpeg.org/ffmpeg-all.html#hls-3

### http

https://www.ffmpeg.org/ffmpeg-all.html#http

### rtmp

https://www.ffmpeg.org/ffmpeg-all.html#rtmp

### rtp

https://www.ffmpeg.org/ffmpeg-all.html#rtp

### rtsp

https://www.ffmpeg.org/ffmpeg-all.html#rtsp

### tcp

https://www.ffmpeg.org/ffmpeg-all.html#tcp

### udp

https://www.ffmpeg.org/ffmpeg-all.html#udp

## Resampleer Options

- -in_channel_count: Set the number of input channels.
- -out_channel_count: Set the number of output channels.
- -used_channel_count: Set the number of used input channels.
- -in_sample_rate: Set the input sample rate.
- -out_sample_rate: Set the output sample rate.
- -in_sample_fmt: Specify the input sample format. 
- -out_sample_fmt: Specify the output sample format.
- -in_channel_layout: Set the input channel layout.
- -out_channel_layout: Set the output channel layout.
- -output_sample_bits: set number of used output sample bits for dithering.

## Scaler Options

- -srcw: Set source width.
- -srch: Set source height.
- -dstw: Set destination width.
- -dsth: Set destination height.
- -src_format: Set source pixel format 
- -dst_format: Set destination pixel format
- -src_range: If value is set to `1`, indicates source is full range. Default value is `0`, which indicates source is limited range.
- -dst_range: If value is set to `1`, enable full range for destination. Default value is `0`, which enables limited range.

