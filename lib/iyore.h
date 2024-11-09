#ifndef IYORE_H
#define IYORE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <cjson/cJSON.h>
// #include "cJSON.h"
#include <unistd.h>
#include <fcntl.h> // struct stat fileStat;
#include <termios.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/stat.h>

typedef struct
{
    int userId;
    char username[50];
    char password[50];
} User;

void printIYORE(void);
void printLogin(void);
void printRegister(void);

void fullClear(void);
void lClear(int n);

void putChar(char c);
void putStr(const char *s);
void putNbr(int nb);

char getChar(void);
void getStr(char *s, size_t maxLen);
int generateUniqueId(void);
void storeUserData(User newUser, char *fileName);

void renderText(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, const char *text, int x, int y);
void renderImg(SDL_Renderer *renderer, SDL_Surface *img, int x, int y, int width, int height);
int isMouseOverButton(SDL_Rect *button, int mouseX, int mouseY);
int checkUserExists(const char *username, const char *fileName);
int isUserRegistered(const char *username, const char *password, const char *fileName);

size_t ft_strlen(const char *s);
char *ft_strchr(char *str, char c);
char *ft_strstr(char *str, char *to_find);
char *ft_strncat(char *dest, const char *src, size_t n);
void ft_strncpy(char *dest, const char *src, size_t n);
void *ft_memset(void *b, int c, size_t len);
char *ft_strdup(const char *s);
int ft_strcmp(const char *s1, const char *s2);

#endif
