

#ifndef SDL_LCD__H
#define SDL_LCD__H

#include  "SDL2/SDL.h"

#include  <stdio.h>

int init_SDL2(void);
void SDL_DrawPoint ( uint16_t x, uint16_t y, uint16_t c );
void SDL_refresh(void);
void SDL_close(void);


int get_SDL_Event(void);
int tick_thread(void * data);

#endif // SDL_LCD__H

