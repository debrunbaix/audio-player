#ifndef MP3_PLAYER_H
#define MP3_PLAYER_H

#include <mpg123.h>
#include "audio_format.h"

/**
 * Initialize MP3 player with the given file.
 */
int init_mp3_player(const char *file_path, AudioPlayer *player);

/**
 * Play MP3 audio.
 */
int play_mp3(AudioPlayer *player);

/**
 * Cleanup MP3 player resources.
 */
void cleanup_mp3_player(AudioPlayer *player);

#endif
