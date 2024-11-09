#ifndef MENU_H
#define MENU_H

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 420

#define FILE_NAME "users.json"

// extern declarations of variables
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern TTF_Font *titleFont;
extern TTF_Font *topicsFont;
extern TTF_Font *textfont;
extern TTF_Font *smallfont;
extern TTF_Font *hidingPasswordFont;
extern SDL_Surface *eyeImgSF;
extern SDL_Surface *eyeSlashImgSF;
extern SDL_Surface *IRiconImgSF;


void menu(void);
int Register(void);
int Login(void);

#endif
