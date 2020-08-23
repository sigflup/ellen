
#include <stdio.h>
#include <stdlib.h>


#include "SDL.h"
#include "SDL_opengles2.h"
#include "link.h"
#include "util.h"

GLuint load_tex(char *filename, int mode) {
 SDL_RWops *rw;
 SDL_Surface *pic;
 char *buffer;
 unsigned char r,g,b,a;
 int i,j,k;
 void *data;
 int readme;
 GLuint ret;
 rw = SDL_RWFromFile(filename, "r");
if((pic = IMG_Load_RW(rw, 0))==0) exit(0);
 SDL_RWclose(rw);
if(pic->format->BytesPerPixel == 1) exit(0);
 buffer = (unsigned char *)pic->pixels;

 glGenTextures(1, &ret);
 glBindTexture(GL_TEXTURE_2D, ret);
 glTexImage2D(GL_TEXTURE_2D, 0, (pic->format->Amask == 0 ? GL_RGB : GL_RGBA),
   pic->w, pic->h, 0, (pic->format->Amask == 0 ? GL_RGB : GL_RGBA),
   GL_UNSIGNED_BYTE, buffer);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
/* if(mode == CLAMP) {
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP);
 } else {
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
 } 
*/ 
// SDL_FreeSurface(pic);
 return ret;
}


GLuint load_jpg_tex(char *filename, int mode) {
 SDL_RWops *rw;
 SDL_Surface *pic;
 char *buffer;
 unsigned char r,g,b,a;
 int i,j,k;
 void *data;
 int readme;
 GLuint ret;
 rw = SDL_RWFromFile(filename, "r");
if((pic = IMG_LoadJPG_RW(rw))==0) return;
 SDL_RWclose(rw);
if(pic->format->BytesPerPixel == 1) exit(0);
 buffer = (unsigned char *)pic->pixels;

 glGenTextures(1, &ret);
 glBindTexture(GL_TEXTURE_2D, ret);
 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
   pic->w, pic->h, 0, GL_RGB ,
   GL_UNSIGNED_BYTE, buffer);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
/* if(mode == CLAMP) {
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP);
 } else {
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
 } 
*/ 
// SDL_FreeSurface(pic);
 return ret;
}


