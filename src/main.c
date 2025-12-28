#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "audio_player.h"

AudioFormat detect_audio_format(const char *file_path)
{
  const char *ext = strrchr(file_path, '.');
  if (ext == NULL) return FORMAT_UNKNOWN;

  if (strcasecmp(ext, ".mp3") == 0) return FORMAT_MP3;
  if (strcasecmp(ext, ".wav") == 0) return FORMAT_WAV;

  return FORMAT_UNKNOWN;
}

const char *get_format_name(AudioFormat format)
{
  switch (format) {
    case FORMAT_MP3: return "MP3";
    case FORMAT_WAV: return "WAV";
    default: return "Unknown";
  }
}

void print_banner(const char *file_path)
{
  printf("====================\n");
  printf("--- Audio Player ---\n");
  printf("====================\n");
  printf("\n  Starting with : %s\n", file_path);
}

int init_audio_player(const char *file_path, AudioPlayer *player)
{
  player->format = detect_audio_format(file_path);

  switch (player->format) {
    case FORMAT_MP3:
      return init_mp3_player(file_path, player);
    case FORMAT_WAV:
      return init_wav_player(file_path, player);
    default:
      printf("[x] Unsupported audio format.\n");
      printf("    Supported formats: MP3, WAV\n");
      return ERROR;
  }
}

int play_audio(AudioPlayer *player)
{
  switch (player->format) {
    case FORMAT_MP3:
      return play_mp3(player);
    case FORMAT_WAV:
      return play_wav(player);
    default:
      return ERROR;
  }
}

void cleanup_audio_player(AudioPlayer *player)
{
  switch (player->format) {
    case FORMAT_MP3:
      cleanup_mp3_player(player);
      break;
    case FORMAT_WAV:
      cleanup_wav_player(player);
      break;
    default:
      break;
  }
}

int launch_audio_player(const char *file_path)
{
  AudioPlayer player = {0};

  if (init_audio_player(file_path, &player) != SUCCESS) return ERROR;

  if (play_audio(&player) != SUCCESS)
  {
    cleanup_audio_player(&player);
    return ERROR;
  }

  cleanup_audio_player(&player);

  return SUCCESS;
}

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    printf("Usage: %s <audio_file>\n", argv[0]);
    printf("Supported formats: MP3, WAV\n");
    return 0;
  }
  const char *file_path = argv[1];
  print_banner(file_path);
  if (launch_audio_player(file_path) != SUCCESS) return ERROR;
  return SUCCESS;
}
