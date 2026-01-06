#ifndef FLAC_PLAYER_H
#define FLAC_PLAYER_H

#include <sndfile.h>
#include "audio_format.h"

/**
 * Initialize FLAC player with the given file.
 */
int init_flac_player(const char *file_path, AudioPlayer *player);

/**
 * Play FLAC audio.
 */
int play_flac(AudioPlayer *player);

/**
 * Cleanup FLAC player resources.
 */
void cleanup_flac_player(AudioPlayer *player);

#endif
