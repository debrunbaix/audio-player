#include <stdio.h>
#include <stdlib.h>
#include <mpg123.h>
#include <ao/ao.h>
#include "audio_player.h"

void print_banner(char *file_path)
{
  printf("====================\n");
  printf("--- Audio Player ---\n");
  printf("====================\n");
  printf("\n  Starting with : %s.\n", file_path);
}

int init_audio_player(char *file_path, AudioPlayer *player)
{
  int numChannels;
  int encoding;
  unsigned int bitRate;
  ao_sample_format format;
  int driver;

  player->mp3Handle = mpg123_new(NULL, NULL);
  if (player->mp3Handle == NULL)
  {
    printf("[x] Failed to create mpg123 handle.\n");
    return ERROR;
  }

  if (mpg123_open(player->mp3Handle, file_path) != MPG123_OK)
  {
    printf("[x] Failed to open MP3 file.\n");
    mpg123_delete(player->mp3Handle);
    return ERROR;
  }

  if (mpg123_getformat(player->mp3Handle, (long*)&bitRate, &numChannels, &encoding) == MPG123_ERR)
  {
    printf("[x] No MP3 file provided.\n");
    mpg123_close(player->mp3Handle);
    mpg123_delete(player->mp3Handle);
    return ERROR;
  }

  player->mp3BufferSize = mpg123_outblock(player->mp3Handle);
  player->mp3Buffer = (unsigned char*) malloc(player->mp3BufferSize * sizeof(unsigned char));
  if (player->mp3Buffer == NULL) {
    printf("[x] Failed to allocate buffer.\n");
    mpg123_close(player->mp3Handle);
    mpg123_delete(player->mp3Handle);
    return ERROR;
  }

  printf("\nInfos :\n");
  printf("- BitRate : %d\n", bitRate);
  if (numChannels == 1) printf("- Channels : Mono\n");
  if (numChannels == 2) printf("- Channels : Stereo\n");

  ao_initialize();

  driver = ao_default_driver_id();

  format.bits = mpg123_encsize(encoding) * BITS;
  format.rate = bitRate;
  format.channels = numChannels;
  format.byte_format = AO_FMT_NATIVE;
  format.matrix = 0;

  player->dev = ao_open_live(driver, &format, NULL);
  if (player->dev == NULL) {
    printf("[x] Failed to open audio device.\n");
    free(player->mp3Buffer);
    mpg123_close(player->mp3Handle);
    mpg123_delete(player->mp3Handle);
    ao_shutdown();
    return ERROR;
  }
  return SUCCESS;
}

int play_audio(AudioPlayer *player)
{
  size_t done;

  while (mpg123_read(player->mp3Handle, player->mp3Buffer, player->mp3BufferSize, &done) == MPG123_OK)
  {
    ao_play(player->dev, (char*)player->mp3Buffer, done);
  }

  return SUCCESS;
}

void cleanup_audio_player(AudioPlayer *player)
{
  if (player->mp3Buffer) free(player->mp3Buffer);
  if (player->dev) ao_close(player->dev);
  if (player->mp3Handle)
  {
    mpg123_close(player->mp3Handle);
    mpg123_delete(player->mp3Handle);
  }
  ao_shutdown();
}

int launch_audio_player(char *file_path)
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
    printf("[-] No audio file provided.\n");
    return 0;
  }
  char *file_path = argv[1];
  print_banner(file_path);
  if (launch_audio_player(file_path) != SUCCESS) return ERROR;
  return SUCCESS;
}
