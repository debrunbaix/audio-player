#ifndef WAV_PLAYER_H
#define WAV_PLAYER_H

#include <sndfile.h>
#include "audio_format.h"

/**
 * Initialize WAV player with the given file.
 */
int init_wav_player(const char *file_path, AudioPlayer *player);

/**
 * Play WAV audio.
 */
int play_wav(AudioPlayer *player);

/**
 * Cleanup WAV player resources.
 */
void cleanup_wav_player(AudioPlayer *player);

#endif
