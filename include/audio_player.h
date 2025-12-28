#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include "audio_format.h"
#include "mp3_player.h"
#include "wav_player.h"

/**
 * Display the program's banner and the audio file actually playing.
 */
void print_banner(const char *file_path);

/**
 * Initialize audio player based on file format.
 */
int init_audio_player(const char *file_path, AudioPlayer *player);

/**
 * Play the audio file.
 */
int play_audio(AudioPlayer *player);

/**
 * Clean up resources.
 */
void cleanup_audio_player(AudioPlayer *player);

/**
 * Main function to manage the audio player.
 */
int launch_audio_player(const char *file_path);

#endif
