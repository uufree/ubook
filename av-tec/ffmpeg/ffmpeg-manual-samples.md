# FFmpeg 4.4 Manual Samples

- 视频

  - muxer、demuxer

    - 视频格式转换

      ```c
      // H264视频转ts视频流
      ffmpeg -i test.h264 -vcodec copy -f mpegts test.ts    
      // H264视频转mp4
      ffmpeg -i test.h264 -vcodec copy -f mp4 test.mp4
      // ts视频转mp4    
      ffmpeg -i test.ts -acodec copy -vcodec copy -f mp4 test.mp4
      // mp4视频转flv
      ffmpeg -i test.mp4 -acodec copy -vcodec copy -f flv test.flv     
      ```

      

    - 分离、合并音视频流

    - 截取视频片段

    - 视频切片

  - codec、decodec

    - 视频截图
    - 视频拼接
    - 图片尺寸修改
    - 视频拆分图片
    - 图片合成视频
    - 视频录制

  - filter

    - 视频加水印
    - 视频旋转
    - 视频色调调整（黑白、曝光等）
    - 视频尺寸裁剪
    - 视频缩放
    - 视频去水印
    - 视频加边框

  - network

    - udp视频流推送
    - tcp视频流推送
    - rtmp视频流推送

  - quality

    - 计算PSNR
    - 计算SSIM

- 音频

  - muxer、demuxer
    - 音频封装格式转换
    - 截取音频片段
    - 音频分片
  - codec、decodec
    - PCM与各种格式互转
    - 音频重采样。支持：声道、采样位数、采样频率等的相互转换
  - filter
    - 音频倍速（0-n倍）播放

- 字幕

  - 字幕格式文件转换
  - 从视频中拆分字幕
  - 将字幕集成到视频中