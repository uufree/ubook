#ifndef DECODE_H
#define DECODE_H

enum DecodeType {
  Video,
  Audio,
  NBTypes
};

int decode(const char* media_path, DecodeType type);

#endif