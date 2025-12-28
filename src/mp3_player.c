#include <stdio.h>
#include <stdlib.h>
#include <mpg123.h>
#include <ao/ao.h>
#include "mp3_player.h"

int init_mp3_player(const char *file_path, AudioPlayer *player)
{
  int numChannels;
  int encoding;
  long bitRate;
  ao_sample_format format;
  int driver;
  mpg123_handle *mp3Handle;

  mp3Handle = mpg123_new(NULL, NULL);
  if (mp3Handle == NULL)
  {
    printf("[x] Failed to create mpg123 handle.\n");
    return ERROR;
  }

  if (mpg123_open(mp3Handle, file_path) != MPG123_OK)
  {
    printf("[x] Failed to open MP3 file.\n");
    mpg123_delete(mp3Handle);
    return ERROR;
  }

  if (mpg123_getformat(mp3Handle, &bitRate, &numChannels, &encoding) == MPG123_ERR)
  {
    printf("[x] Invalid MP3 file.\n");
    mpg123_close(mp3Handle);
    mpg123_delete(mp3Handle);
    return ERROR;
  }

  player->bufferSize = mpg123_outblock(mp3Handle);
  player->buffer = (unsigned char*) malloc(player->bufferSize * sizeof(unsigned char));
  if (player->buffer == NULL) {
    printf("[x] Failed to allocate buffer.\n");
    mpg123_close(mp3Handle);
    mpg123_delete(mp3Handle);
    return ERROR;
  }

  printf("\nInfos :\n");
  printf("- Format : MP3\n");
  printf("- BitRate : %ld\n", bitRate);
  if (numChannels == 1) printf("- Channels : Mono\n");
  if (numChannels == 2) printf("- Channels : Stereo\n");

  ao_initialize();

  driver = ao_default_driver_id();

  format.bits = mpg123_encsize(encoding) * BITS;
  format.rate = (int)bitRate;
  format.channels = numChannels;
  format.byte_format = AO_FMT_NATIVE;
  format.matrix = 0;

  player->dev = ao_open_live(driver, &format, NULL);
  if (player->dev == NULL) {
    printf("[x] Failed to open audio device.\n");
    free(player->buffer);
    mpg123_close(mp3Handle);
    mpg123_delete(mp3Handle);
    ao_shutdown();
    return ERROR;
  }

  player->handle = mp3Handle;
  player->format = FORMAT_MP3;

  return SUCCESS;
}

int play_mp3(AudioPlayer *player)
{
  size_t done;
  mpg123_handle *mp3Handle = (mpg123_handle*)player->handle;

  while (mpg123_read(mp3Handle, player->buffer, player->bufferSize, &done) == MPG123_OK)
  {
    ao_play(player->dev, (char*)player->buffer, done);
  }

  return SUCCESS;
}

void cleanup_mp3_player(AudioPlayer *player)
{
  mpg123_handle *mp3Handle = (mpg123_handle*)player->handle;

  if (player->buffer) free(player->buffer);
  if (player->dev) ao_close(player->dev);
  if (mp3Handle)
  {
    mpg123_close(mp3Handle);
    mpg123_delete(mp3Handle);
  }
  ao_shutdown();
}
