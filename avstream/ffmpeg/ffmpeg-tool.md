# FFmpeg Documentation

[TOC]

## 语法格式

```c
ffmpeg [global_options] {[input_file_options] -i input_url} ... {[output_file_options] output_url} ...
```

## 简介

FFmpeg是一个非常快的音视频编解码器，可以处理离线、实时的音视频。

使用demo：

```c
// 码率设置。将输出视频的码率设置为64 kbit/s
ffmpeg -i input.avi -b:v 64k -bufsize 64k output.avi
    
// 帧率设置。将输出视频的帧率设置为24帧/秒
ffmpeg -i input.avi -r 24 output.avi
```

## 详细说明

ffmpeg通用工作流程：

- **demuxer**和**muxer**：使用<libavformat>
- **decoder**和**encoder**：使用<libavcodec>
- **filter**：使用<libavfilter>
- **audio process**：使用<libswresample>
- **image process**：使用<libscale>

```c
 _______              ______________
|       |            |              |
| input |  demuxer   | encoded data |   decoder
| file  | ---------> | packets      | -----+
|_______|            |______________|      |
                                           v
                                       _________
                                      |         |
                                      | decoded |
                                      | frames  |
                                      |_________|
 ________             ______________       |
|        |           |              |      |
| output | <-------- | encoded data | <----+
| file   |   muxer   | packets      |   encoder
|________|           |______________|
  
// 过滤流程     
 _________                        ______________
|         |                      |              |
| decoded |                      | encoded data |
| frames  |\                   _ | packets      |
|_________| \                  /||______________|
             \   __________   /
  simple     _\||          | /  encoder
  filtergraph   | filtered |/
                | frames   |
                |__________|
     
// 简单过滤流程
 _______        _____________        _______        ________
|       |      |             |      |       |      |        |
| input | ---> | deinterlace | ---> | scale | ---> | output |
|_______|      |_____________|      |_______|      |________|
     
// 复杂过滤流程
 _________
|         |
| input 0 |\                    __________
|_________| \                  |          |
             \   _________    /| output 0 |
              \ |         |  / |__________|
 _________     \| complex | /
|         |     |         |/
| input 1 |---->| filter  |\
|_________|     |         | \   __________
               /| graph   |  \ |          |
              / |         |   \| output 1 |
 _________   /  |_________|    |__________|
|         | /
| input 2 |/
|_________|     
     
// 流拷贝流程
 _______              ______________            ________
|       |            |              |          |        |
| input |  demuxer   | encoded data |  muxer   | output |
| file  | ---------> | packets      | -------> | file   |
|_______|            |______________|          |________|     
```



