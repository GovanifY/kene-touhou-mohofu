#ifndef _FPS_H_
#define _FPS_H_

#include "support.h"

extern unsigned int psp_get_uint32_ticks(void);
extern void fps_init(void);
extern void fps_newframe(void);

//#define TICK_INTERVAL 28
//#define PIXEL_SPEED 50
//enum /*_fps_methods_*/ { WAITFORFRAME, MAXFPS };

//extern void toggle_fps(void);
//extern void fps_show(void);
//extern Uint32 TimeLeft(void);
//extern void waitfornextframe(void);

#endif /* _FPS_H_ */
