CC = gcc # C Compiler
CFLAGS = -Wall -Wextra -Werror -g # Compiler Flags with debug symbols
SRC = main.c iyore.c menu.c
OBJ = $(SRC:.c=.o) # replace .c with .o from SRC => main.o iyore.o menu.o
TARGET = IYORE # a.out
LIBS = -lSDL2 -lSDL2_ttf -lSDL2_image -lcjson

all: clean.o $(TARGET) clean

clean.o:
	rm -f *.o

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LIBS)

clean:
	rm -f $(OBJ)
