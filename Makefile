CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -lmpg123 -lao

TARGET = audio_player
SRC = main.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.c audio_player.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(TARGET)

re: fclean all

.PHONY: all clean fclean re
