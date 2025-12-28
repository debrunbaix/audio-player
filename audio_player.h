#ifndef AUDIO_PLAYER
#define AUDIO_PLAYER

#define BITS 8
#define SUCCESS 0
#define ERROR -1

typedef struct {
  mpg123_handle *mp3Handle;
  ao_device *dev;
  unsigned char *mp3Buffer;
  size_t mp3BufferSize;
} AudioPlayer;

/**
  * Function that display the program's banner and the audio file actually playing.
  */
void print_banner(char *file_path);

/**
  * Function to initiate mpg123 & ao library and verify the input file.
  */ 
int init_audio_player(char *file_path, AudioPlayer *player);

/**
  * Function to play the audio file with libao.
  */ 
int play_audio(AudioPlayer *player);

/**
  * Function to clean the heap and the library.
  */ 
void cleanup_audio_player(AudioPlayer *player);

/**
  * Function to manage the audio_player.
  */ 
int launch_audio_player(char *file_path);

#endif
