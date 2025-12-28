#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include <ao/ao.h>
#include "wav_player.h"

#define WAV_BUFFER_SIZE 4096

int init_wav_player(const char *file_path, AudioPlayer *player)
{
  SF_INFO sfinfo;
  SNDFILE *sndfile;
  ao_sample_format format;
  int driver;

  sfinfo.format = 0;

  sndfile = sf_open(file_path, SFM_READ, &sfinfo);
  if (sndfile == NULL)
  {
    printf("[x] Failed to open WAV file: %s\n", sf_strerror(NULL));
    return ERROR;
  }

  player->bufferSize = WAV_BUFFER_SIZE * sfinfo.channels * sizeof(short);
  player->buffer = (unsigned char*) malloc(player->bufferSize);
  if (player->buffer == NULL) {
    printf("[x] Failed to allocate buffer.\n");
    sf_close(sndfile);
    return ERROR;
  }

  printf("\nInfos :\n");
  printf("- Format : WAV\n");
  printf("- SampleRate : %d\n", sfinfo.samplerate);
  if (sfinfo.channels == 1) printf("- Channels : Mono\n");
  if (sfinfo.channels == 2) printf("- Channels : Stereo\n");

  ao_initialize();

  driver = ao_default_driver_id();

  format.bits = 16;
  format.rate = sfinfo.samplerate;
  format.channels = sfinfo.channels;
  format.byte_format = AO_FMT_NATIVE;
  format.matrix = 0;

  player->dev = ao_open_live(driver, &format, NULL);
  if (player->dev == NULL) {
    printf("[x] Failed to open audio device.\n");
    free(player->buffer);
    sf_close(sndfile);
    ao_shutdown();
    return ERROR;
  }

  player->handle = sndfile;
  player->format = FORMAT_WAV;

  return SUCCESS;
}

int play_wav(AudioPlayer *player)
{
  sf_count_t readCount;
  SNDFILE *sndfile = (SNDFILE*)player->handle;

  while ((readCount = sf_read_short(sndfile, (short*)player->buffer, WAV_BUFFER_SIZE)) > 0)
  {
    ao_play(player->dev, (char*)player->buffer, readCount * sizeof(short));
  }

  return SUCCESS;
}

void cleanup_wav_player(AudioPlayer *player)
{
  SNDFILE *sndfile = (SNDFILE*)player->handle;

  if (player->buffer) free(player->buffer);
  if (player->dev) ao_close(player->dev);
  if (sndfile) sf_close(sndfile);
  ao_shutdown();
}
