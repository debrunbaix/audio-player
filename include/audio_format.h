#ifndef AUDIO_FORMAT_H
#define AUDIO_FORMAT_H

#include <ao/ao.h>

#define BITS 8
#define SUCCESS 0
#define ERROR -1

typedef enum {
  FORMAT_UNKNOWN,
  FORMAT_MP3,
  FORMAT_WAV,
  FORMAT_FLAC
} AudioFormat;

typedef struct {
  void *handle;
  ao_device *dev;
  unsigned char *buffer;
  size_t bufferSize;
  AudioFormat format;
} AudioPlayer;

/**
 * Detect audio format from file extension.
 */
AudioFormat detect_audio_format(const char *file_path);

/**
 * Get format name as string.
 */
const char *get_format_name(AudioFormat format);

#endif
