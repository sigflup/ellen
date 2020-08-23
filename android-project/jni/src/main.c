#include <stdio.h>
#include <stdlib.h>


#include "SDL.h"
#include "SDL_opengles2.h"
#include "SDL_image.h"
#include "link.h"
#include "fifo.h"
#include "mp3.h"

SDL_Renderer *screen;


SDL_Window *window;
SDL_GLContext context;
int width, height;
int song_len = 1;
int tick = 0;

SDL_Texture *head, *body, *path, *cool, *intro;
Uint32 start_tick;
int intro_alpha = 255;


void audio(void *userdata, Uint8 *stream, int len) {
 int i;
 for(i=0;i<len;i++)  
  stream[i] = read_char_fifo(&decode_fifo);
 if(tick>=song_len)
  exit(0);
 tick++;
}

int decode_thread(void *ptr) {
 uncompress_mp3("song.mp3"); 
}

SDL_Texture *load_t(char *in ) { 
 SDL_Texture *ret;
 SDL_Surface *tmp; 
 tmp = IMG_Load(in);
 ret = SDL_CreateTextureFromSurface(screen, tmp);
 SDL_FreeSurface(tmp);
 return ret;
}

void draw(void) {
 SDL_Rect src, dst, center;
 SDL_SetRenderDrawColor(screen,
		 (cos((float)SDL_GetTicks()/500.0)*255.0f),
		 (cos((float)(SDL_GetTicks()/500.0) + (M_PI/2.0f)) * 255.0f),
		 (cos((float)(SDL_GetTicks()/500.0) + (M_PI)) * 255.0f),
		 0);

 SDL_SetRenderDrawBlendMode(screen , SDL_BLENDMODE_NONE); 
 SDL_RenderClear(screen);

 dst.x = 0;
 dst.y = 0;
 dst.w = width;
 dst.h = height;

 SDL_RenderCopyEx(screen, path, NULL, &dst, SDL_GetTicks()/50.0f, NULL, SDL_FLIP_NONE);

 SDL_RenderCopyEx(screen, path, NULL, &dst, SDL_GetTicks()/20.0f, NULL, SDL_FLIP_NONE);




 dst.x = 0;
 dst.y = cos((float)SDL_GetTicks()/100.0f) * 20.0f; 

 dst.w = width; 
 dst.h = height;
 SDL_RenderCopy(screen, body, NULL, &dst);

 dst.y = cos(((float)SDL_GetTicks()/100.0f)+(M_PI/2.0f)) * 30.0f; 
 dst.y+= height /40;
 SDL_RenderCopy(screen, head, NULL, &dst);

 SDL_RenderCopy(screen, cool, NULL, NULL);


 SDL_SetRenderDrawBlendMode(screen , SDL_BLENDMODE_BLEND); 
 SDL_SetTextureAlphaMod(intro, intro_alpha); 
 SDL_RenderCopy(screen, intro, NULL, NULL);

 if(intro_alpha!=0) {
  if((SDL_GetTicks() - start_tick) > 20*1000) intro_alpha--; 
 }
 

 SDL_RenderPresent(screen);
}

int SDL_main(int argc, char **argv) {
 SDL_AudioSpec req, ret;

 SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
 IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
 
 /*
 SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
 SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
 SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
*/
 window = SDL_CreateWindow("ellen", 0,0,0,0, 
   SDL_WINDOW_FULLSCREEN_DESKTOP);
 
 screen = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

 SDL_GetWindowSize(window, &width, &height);

 SDL_CreateThread(decode_thread, "decode_thread", NULL);

 SDL_Delay(500);

 req.freq = 48000;
 req.channels=2;
 req.format= AUDIO_S16LSB;
 req.samples=0x7fff;
 req.callback = audio;

 head = load_t("head.png"); 
 body = load_t("body.png");
 path = load_t("path.png");
 cool = load_t("cool.png");
 intro= load_t("intro.png");

 SDL_SetRenderDrawColor(screen,0,200,0,0);
 SDL_PauseAudioDevice( SDL_OpenAudioDevice(NULL, 0, &req, &ret, 0), 0);

 start_tick = SDL_GetTicks();
 for(;;) { 
  draw();
  if((SDL_GetTicks() - start_tick) > (155*1000)) exit(0);
  SDL_Delay(10);
 } 

 
}
