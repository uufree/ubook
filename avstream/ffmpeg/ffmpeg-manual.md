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

### Stream Mapping

```
ffmpeg version ... Copyright (c) 2000-2012 the FFmpeg developers
...
Input #0, matroska,webm, from 'input.mkv':
  Duration: 01:39:44.02, start: 0.000000, bitrate: 5793 kb/s
    Stream #0:0(eng): Video: h264 (High), yuv420p, 1920x800, 23.98 fps, 23.98 tbr, 1k tbn, 47.95 tbc (default)
    Stream #0:1(ger): Audio: dts (DTS), 48000 Hz, 5.1(side), s16, 1536 kb/s (default)
    Stream #0:2(eng): Audio: dts (DTS), 48000 Hz, 5.1(side), s16, 1536 kb/s
    Stream #0:3(ger): Subtitle: text (default)

```

- 示例1：

  ```
  我们想要：
  将视频流复制
  将德语音频流编码为MP3（128kbps）和AAC（96kbps）（在输出中创建两个音频流）
  将英语音频流删除
  将字幕流复制
  
  ffmpeg -i input.mkv \
      -map 0:0 -map 0:1 -map 0:1 -map 0:3 \
      -c:v copy \
      -c:a:0 libmp3lame -b:a:0 128k \
      -c:a:1 libfaac -b:a:1 96k \
      -c:s copy \
      output.mkv
  ```

- 示例2：

  ```
  我们想要：
  倒序排列输入流
  
  ffmpeg -i input.mkv -map 0:3 -map 0:2 -map 0:1 -map 0:0 -c copy output.mkv
  ```

- 示例3：

  ```
  我们想从同一个输入文件中仅提取音频流
  
  ffmpeg -i input.mkv -map 0:1 -map 0:2 -c copy output.mkv
  ```

- 示例4：

  ```
  可以使用"-map"命令来创建多路文件输出
  
  ffmpeg -i input.mkv -map 0:1 -map 0:2 audios_only.mkv -map 0:0 video_only.mkv
  ```

### Stream Options

```cassandra
-command : stream_type : stream_index

// 对container中的第2个audio stream应用ac3编码方式
-codec:a:1 ac3     
// 对container中的audio stream设置128 kbits
-b:a 128k
// 拷贝container中的所有stream   
-codec copy
// 对container中的第2个stream设置线程数量为4    
-threads:1 4    
```

- stream type
  - `v`: video
  - `a`: audio
  - `s`: subtitle 
- stream index: 0~n

### Generic Options

- -L: Show license.
- -h: Show help.  Possible values of arg are:
  - long: Print advanced tool options in addition to the basic tool options.
  - full: Print complete list of options, including shared and private options for encoders, decoders, demuxers, muxers, filters, etc.
  - decoder=decoder_name: Print detailed information about the decoder named decoder_name. Use the -decoders option to get a list of all decoders.
  - encoder=encoder_name: Print detailed information about the encoder named encoder_name. Use the -encoders option to get a list of all encoders.
  - demuxer=demuxer_name: Print detailed information about the demuxer named demuxer_name. Use the -formats option to get a list of all demuxers and muxers.
  - muxer=muxer_name: Print detailed information about the muxer named muxer_name. Use the -formats option to get a list of all muxers and demuxers.
  - filter=filter_name: Print detailed information about the filter named filter_name. Use the -filters option to get a list of all filters.
  - bsf=bitstream_filter_name: Print detailed information about the bitstream filter named bitstream_filter_name. Use the -bsfs option to get a list of all bitstream filters.
  - protocol=protocol_name: Print detailed information about the protocol named protocol_name. Use the -protocols option to get a list of all protocols.
- -version: Show version.
- -buildconf: Show the build configuration, one option per line.
- -formats: Show available formats (including devices).
- -demuxers: Show available demuxers.
- -muxers: Show available muxers.
- -devices: Show available devices.
- -codecs: Show all codecs known to libavcodec.
- -decoders: Show available decoders.
- -encoders: Show all available encoders.
- -bsfs: Show available bitstream filters.
- -protocols: Show available protocols.
- -filters: Show available libavfilter filters.
- -pix_fmts: Show available pixel formats.
- -sample_fmts: Show available sample formats.
- -layouts: Show channel names and standard channel layouts.
- -colors: Show recognized color names.
- -sources: Show autodetected sources of the input device.
- -sinks: Show autodetected sinks of the output device.
- -loglevel: Set logging level and flags used by the library.
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

- -f fmt (input/output): Force input or output file format.  so this option is not needed in most cases.

- -i url (input): input file url

- -y (global): Overwrite output files without asking.

- -n (global): Do not overwrite output files, and exit immediately if a specified output file already exists.

- -c codec: Select an encoder (when used before an output file) or a decoder (when used before an input file) for one or more streams. codec is the name of a decoder/encoder or a special value copy (output only) to indicate that the stream is not to be re-encoded.

  ```cassandra
  ffmpeg -i INPUT -c:v libx264 -c:a copy OUTPUT
  ffmpeg -i INPUT -c copy -c:v:1 libx264 -c:a:137 libvorbis OUTPUT
  ```

- -t duration (input/output): 

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

- -to position (input/output): Stop writing the output or reading the input at position. position must be a time duration specification

  ```cassandra
  "55": 55 seconds
  "0.2":  0.2 seconds
  "200ms": 200 milliseconds, that’s 0.2s
  "200000us": 200000 microseconds, that’s 0.2s
  "12:03:45: 12 hours, 03 minutes and 45 seconds
  "23.189":  23.189 seconds
  ```

- -fs limit_size (output): Set the file size limit, expressed in bytes. No further chunk of bytes is written after the limit is exceeded. The size of the output file is slightly more than the requested file size.

- -ss position (input/output): 

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

- -itsoffset offset: Set the input time offset.

- -timestamp date (output): Set the recording timestamp in the container.

  ```cassandra
  [(YYYY-MM-DD|YYYYMMDD)[T|t| ]]((HH:MM:SS[.m...]]])|(HHMMSS[.m...]]]))[Z]
  now
  ```

- -metadata[:metadata_specifier] key=value (output,per-metadata): Set a metadata key/value pair.

  ```
  ffmpeg -i in.avi -metadata title="my title" out.flv
  ffmpeg -i INPUT -metadata:s:a:0 language=eng OUTPUT
  ```

- frames[:stream_specifier] framecount (output,per-stream)：Stop writing to the stream after framecount frames.

  - `a`：audio
  - `v`：video

- -filter[:stream_specifier] filtergraph (output,per-stream): Create the filtergraph specified by filtergraph and use it to filter the stream.

- -pre[:stream_specifier] preset_name (output,per-stream): Specify the preset for matching stream(s).

### Advanded Main Options

### Video Options

### Advanced Video Options

### Audio Options

### Advanced Audio Options

### Subtitle Options

### Advanced Subtitle Options



