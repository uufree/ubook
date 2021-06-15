# FFmpeg Filters

[TOC]

## Audio Filters

To be continue...

## Video Filters

### addroi

标记在视频区域中比较感兴趣的位置。

有以下必填项：

- x
- y
- w，iw（input frame width）
- h，ih（input frame height）
- qoffset：编码区域的量化参数。可选范围：-1～+1
  - 负值意味着更好的质量
  - 正值意味着更差的质量

示例如下：

```cassandra
// 在（0,0）起始，左侧宽100 pix，高全帧的区域内，使用更差的编码
ffmpeg -i 20200904-a.mp4 -c:a copy -c:v libx264 -vf addroi=0:0:100:ih:+1/5 -y cut.mp4

// 在（1/4 w， 1/4 h）起始，宽 1/2w pix，高 1/2 pix的区域内，使用更好的编码
ffmpeg -i 20200904-a.mp4 -c:a copy -c:v libx264 -vf addroi=iw/4:ih/4:iw/2:ih/2:-1/10 -y cut.mp4
```

