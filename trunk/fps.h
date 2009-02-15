#ifndef _FPS_H_
#define _FPS_H_

#include <SDL/SDL.h>

#include "support.h"
#include "font.h"

//#define TICK_INTERVAL 28
//#define PIXEL_SPEED 50
//enum _fps_methods { WAITFORFRAME, MAXFPS };


extern unsigned int PSP_GetTicks(void);
extern void fps_init(void);
extern void fps_newframe(void);


//extern void toggle_fps(void);
//extern void fps_show(void);
//extern Uint32 TimeLeft(void);
//extern void waitfornextframe(void);

#endif
