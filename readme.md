# Audio Player

Audio player in C.

## Features

- [x] Play local file
- [ ] Play remote file

- [x] Play MP3 file
- [x] Play WAV file
- [ ] Play FLAC file

## Project Structure

```
audio-player/
├── include/          # Header files
│   ├── audio_format.h
│   ├── audio_player.h
│   ├── mp3_player.h
│   └── wav_player.h
├── src/              # Source files
│   ├── main.c
│   ├── mp3_player.c
│   └── wav_player.c
├── Makefile
└── readme.md
```

## Dependencies

- `libmpg123` - MP3 decoding
- `libao` - Audio output
- `libsndfile` - WAV file handling

## Build

```bash
make
```

## Usage

```bash
./audio_player <path-to-audio-file>
```

### Example Output

```
====================
--- Audio Player ---
====================

  Starting with : music/Finneas-Little_window_live.mp3

Infos :
- Format : MP3
- BitRate : 44100
- Channels : Stereo
```

## Clean

```bash
make clean   # Remove object files
make fclean  # Remove object files and binary
make re      # Rebuild from scratch
```
