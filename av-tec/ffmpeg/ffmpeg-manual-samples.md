# FFmpeg 4.4 Manual Samples

- 分离、合并音视频流

  ```cassandra
  // 抽取视频流
  ffmpeg -i 20200904-a.mp4 -c:v copy -an out.h264
  
  // 抽取音频流
  ffmpeg -i 20200904-a.mp4 -c:a copy -vn out.aac
  
  // 合并音视频流
  ffmpeg -i out.h264 -i out.aac -c:v copy -c:a copy out.mp4
  ```

- 音视频格式转换

  ```cassandra
  // H264视频转ts视频流
  ffmpeg -i test.h264 -c:v copy -f mpegts test.ts    
  
  // H264视频转mp4
  ffmpeg -i test.h264 -c:v copy -f mp4 test.mp4
  
  // ts视频转mp4    
  ffmpeg -i test.ts -c copy -f mp4 test.mp4
  
  // mp4视频转flv
  ffmpeg -i test.mp4 -c copy -f flv test.flv     
  
  // h264 转 yuv
  ffmpeg -i 20200904-a.mp4 -an -pix_fmt yuv420p test.yuv
  
  // yuv 转 h264
  ffmpeg -s 1280x720 -i test.yuv -c:v libx264 -profile high444  -r 25 -y high.h264
  
  // aac格式转MP3
  ffmpeg -i test.aac -c:a libmp3lame -ar 44100 -ac 2 -f mp3 test.mp3
  
  // aac -> pcm
  ffmpeg -i test.aac -ar 44100 -ac 2 test.pcm
  
  // pcm -> wav
  ffmpeg -ar 44100 -ac 2 -f s16le -i terror.pcm terror.wav
  
  // pcm -> aac
  ffmpeg -ar 44100 -ac 2 -f s16le -i terror.pcm terror.aac
  
  // aac格式转MP3，并放大音量
  ffmpeg -i test.aac -c:a libmp3lame -ar 44100 -ac 2 -vol 3000 -f mp3 test.mp3
  ```

- 截取音视频片段

  ```cassandra
  // 从第10s开始，截取20s的视频片段. 
  ffmpeg -i 20200904-a.mp4 -ss 10 -t 20 -c:v copy -c:a copy -y cut.mp4
  
  // 从第10s开始，截取终止点为30s的视频片段
  ffmpeg -i 20200904-a.mp4 -ss 10 -to 30 -c:v copy -c:a copy -y cut.mp4
  
  // 如果视频比较长，可以选择先定位，再解码（这样不太准，建议片段前后预留3s）
  ffmpeg -ss 01:00:00 -i 20200904-a.mp4  -t 20 -c:v copy -c:a copy -y cut.mp4
  
  // 获取视频帧数
  ffprobe -v error -count_frames -select_streams v:0 -show_entries stream=nb_read_frames -of default=nokey=1:noprint_wrappers=1 cut.mp4
  
  // 截取指定帧数的视频
  ffmpeg -i 20200904-a.mp4 -c copy -frames:v 1000 -frames:a cut.mp4
  
  // 截取指定时长的音频片段
  ffmpeg -i test.mp3 -ss 00:01:00 -t 10 one.mp3
  ```

- 视频切片与合并

  ```cassandra
  // 以10s为单位进行视频分片
  ffmpeg -i 20200904-a.mp4 -c copy -f segment -segment_list cut.list -segment_time 10s -segment_list_entry_prefix "file " cut%03d.mp4
  
  // 以100帧为单位进行视频分片
  ffmpeg -i 20200904-a.mp4 -c copy -f segment -segment_list cut.list -segment_frames 100 -segment_list_entry_prefix "file " cut%03d.mp4
  
  // 视频合并
  // 写一个inputs.txt文件，内容如下：
  file cut1.mp4
  file cut2.mp4
  
  ffmpeg -f concat -i cut.list -c copy merge.mp4
  
  // 以10s为单位进行音频分片
  ffmpeg -i test.mp3 -c copy -f segment -segment_list cut.list -segment_time 10s -segment_list_entry_prefix "file " cut%03d.mp3
  
  // 音频合并
  ffmpeg -f concat -i cut.list -c:a libmp3lame  merge.mp3
  ```

- 视频、图片互转

  ```cassandra
  // 视频转图片，使用video stream中的fps抽取图片
  ffmpeg -i 20200904-a.mp4 -f image2 images/%03d.jpg
  
  // 从指定的时间点截取一张图片
  ffmpeg -i 20200904-a.mp4 -ss 00:01:00 -r 1 -t 1s -f image2 test.jpg
  
  // 使用指定帧率抽取图片，并指定图片的分辨率
  ffmpeg -i 20200904-a.mp4 -r 1 -s 500x500 -f image2 images/%03d.jpg
  
  // 图片转视频
  ffmpeg -i images/%03d.jpg -c:v libx264 -r 25 merge.mp4
  
  // 从视频指定处截取100帧，合成gif
  ffmpeg -i 20200904-a.mp4 -ss 00:01:00 -frames:v 100 -pix_fmt bgr8 -f gif test.gif
  
  // 从视频指定处截取10s，合成gif
  ffmpeg -i 20200904-a.mp4 -ss 00:01:00 -t 10s -pix_fmt bgr8 -f gif test.gif
  ```

- 音视频录制

  ```cassandra
  https://zhuanlan.zhihu.com/p/80903121
  ```

- Filters

  - 视频添加、删除水印

    ```cassandra
    // 添加水印
    ffmpeg -i 20200904-a.mp4 -vf "movie=water.jpg,scale=64:48[watermask];[in][watermask] overlay=30:10 [out]" -y out.mp4
    
    // 删除水印, x:y:w:h
    ffmpeg -i out.mp4 -vf delogo=30:10:64:48 in.mp4
    ```

  - 视频旋转

    ```cassandra
    // 顺时针90度
    ffmpeg -i 20200904-a.mp4 -vf transpose=1 cut.mp4
    
    // 逆时针90度
    ffmpeg -i 20200904-a.mp4 -vf transpose=2 cut.mp4
    
    // 顺时针90度 + 水平翻转
    ffmpeg -i 20200904-a.mp4 -vf transpose=3 cut.mp4
    
    // 逆时针90度 + 水平翻转
    ffmpeg -i 20200904-a.mp4 -vf transpose=0 cut.mp4
    
    // 水平翻转
    ffmpeg -i 20200904-a.mp4 -vf hflip cut.mp4
    
    // 垂直翻转
    ffmpeg -i 20200904-a.mp4 -vf vflip cut.mp4
    ```

  - 倍速播放

    ```cassandra
    // 2倍速播放
    ffmpeg -i 20200904-a.mp4 -filter_complex "[0:v]setpts=0.5*PTS; [0:a]atempo=2.0" -map 0:0 -map 0:1 speed.mp4
    
    // 0.5倍速播放
    ffmpeg -i 20200904-a.mp4 -filter_complex "[0:v]setpts=2*PTS; [0:a]atempo=0.5" -map 0:0 -map 0:1 speed2.mp4
    ```

  - 视频裁剪

    ```cassandra
    // 裁剪, w:h:x:y
    ffmpeg -i 20200904-a.mp4 -vf crop=iw/2:ih/2:100:100 cut.mp4
    ```

  - 视频色调调整（黑白、曝光等）

    ```cass
    to be continue...
    ```

  - 视频分辨率缩小、放大

    ```cassandra
    // 缩小
    ffmpeg -i 20200904-a.mp4 -vf scale=iw/2:ih/2 small.mp4
    
    // 放大
    ffmpeg -i 20200904-a.mp4 -vf scale=iw*2:ih*2 big.mp4
    ```

- 直播

  - rtmp推流、拉流

    ```cassandra
    // 推流
    ffmpeg -re -i INPUT.mp4 -c copy -f flv rtmp://0.0.0.0:1935/stream/test
    
    // 本地重新编码之后，进行推流
    ffmpeg -re -i INPUT.mp4 -acodec aac -ac 2 -ar 44100 -b:a 128k -vol 3000 -vcodec libx264 -b:v 800k -s 360*640 -r 24 -f flv rtmp://0.0.0.0:1935/stream/test
    
    // 拉流
    ffplay -i rtmp://0.0.0.0:1935/stream/test
    ```

  - rtsp推流、拉流

    ```cassandra
    to be continue...
    ```

- 视频质量

  ```cassandra
  // 计算视频的psnr
  ffmpeg -i 20200904-a.mp4 -i baseline.mp4 -lavfi psnr -f null -
  
  // 计算视频的ssim
  ffmpeg -i 20200904-a.mp4 -i baseline.mp4 -lavfi ssim -f null -
  ```

- 字幕格式文件转换

- 从视频中拆分字幕

- 将字幕集成到视频中

  ```cassandra
  to be continue...
  ```