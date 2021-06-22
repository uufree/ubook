#ifndef AUDIO_H
#define AUDIO_H

int demuxer_and_transcode_aac_to_pcm(const char *video_path, const char *pcm_path);
int pcm_to_aac(const char *pcm_path, const char *aac_path);
int pcm_to_wav(const char *pcm_path, const char *wav_path);
int pcm_to_mp3(const char *pcm_path, const char *mp3_path);
int muxer_and_transcode_aac_to_mp3(const char *video_path, const char *new_video_path);

#endif