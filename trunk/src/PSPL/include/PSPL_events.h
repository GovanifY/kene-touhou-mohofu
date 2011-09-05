
/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	http://mohou.huuryuu.com/
	-------------------------------------------------------
	PSPL - PSP customised SDL Layer port.
	SDL Copyright (C) 1997-2004 Sam Lantinga.
---------------------------------------------------------*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: PSPL_events.h,v 1.11 2004/08/20 18:57:01 slouken Exp $";
#endif

#ifdef _SDL_H
error! "このプログラムは SDL.h がインクルードされていると、コンパイルできません。"
#endif

#ifdef _SDL_events_h
error! "このプログラムは SDL_events.h がインクルードされていると、コンパイルできません。"
#endif

/* Include file for SDL event handling */

#ifndef _PSPL_events_h
#define _PSPL_events_h

#include "PSPL_types.h"
//#include "PSPL_active.h"//??
//#include "PSPL_keyboard.h"
//#include "PSPL_mouse.h"
//#include "PSPL_joystick.h"
//#include "PSPL_quit.h"//??

//#inc lude "beg in_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* Event enumerations */
enum
{
	SDL_NOEVENT = 0,		/* Unused (do not remove) */
	/* This last event is only for bounding internal arrays
	  It is the number of bits in the event mask datatype -- u32
	*/
	SDL_NUMEVENTS = 32
};

/* Predefined event masks */
//#define SDL_EVENTMASK(X)	(1<<(X))
//enum
//{
//	SDL_ACTIVEEVENTMASK 	= SDL_EVENTMASK(SDL_ACTIVEEVENT),
//	SDL_KEYDOWNMASK 		= SDL_EVENTMASK(SDL_KEYDOWN),
//	SDL_KEYUPMASK			= SDL_EVENTMASK(SDL_KEYUP),
//	SDL_MOUSEMOTIONMASK 	= SDL_EVENTMASK(SDL_MOUSEMOTION),
//	SDL_MOUSEBUTTONDOWNMASK = SDL_EVENTMASK(SDL_MOUSEBUTTONDOWN),
//	SDL_MOUSEBUTTONUPMASK	= SDL_EVENTMASK(SDL_MOUSEBUTTONUP),
//	SDL_MOUSEEVENTMASK		= SDL_EVENTMASK(SDL_MOUSEMOTION)|
//							  SDL_EVENTMASK(SDL_MOUSEBUTTONDOWN)|
//							  SDL_EVENTMASK(SDL_MOUSEBUTTONUP),
//	SDL_JOYAXISMOTIONMASK	= SDL_EVENTMASK(SDL_JOYAXISMOTION),
//	SDL_JOYBALLMOTIONMASK	= SDL_EVENTMASK(SDL_JOYBALLMOTION),
//	SDL_JOYHATMOTIONMASK	= SDL_EVENTMASK(SDL_JOYHATMOTION),
//	SDL_JOYBUTTONDOWNMASK	= SDL_EVENTMASK(SDL_JOYBUTTONDOWN),
//	SDL_JOYBUTTONUPMASK 	= SDL_EVENTMASK(SDL_JOYBUTTONUP),
//	SDL_JOYEVENTMASK		= SDL_EVENTMASK(SDL_JOYAXISMOTION)|
//							  SDL_EVENTMASK(SDL_JOYBALLMOTION)|
//							  SDL_EVENTMASK(SDL_JOYHATMOTION)|
//							  SDL_EVENTMASK(SDL_JOYBUTTONDOWN)|
//							  SDL_EVENTMASK(SDL_JOYBUTTONUP),
//	SDL_VIDEORESIZEMASK 	= SDL_EVENTMASK(SDL_VIDEORESIZE),
//	SDL_VIDEOEXPOSEMASK 	= SDL_EVENTMASK(SDL_VIDEOEXPOSE),
//	SDL_QUITMASK			= SDL_EVENTMASK(SDL_QUIT),
//	SDL_SYSWMEVENTMASK		= SDL_EVENTMASK(SDL_SYSWMEVENT)
//};
//#define SDL_ALLEVENTS		0xffffffff

/* Application visibility event structure */
typedef struct SDL_ActiveEvent
{
	u8 type; /* SDL_ACTIVEEVENT */
	u8 gain; /* Whether given states were gained or lost (1/0) */
	u8 state;	/* A mask of the focus states */
} SDL_ActiveEvent;





/* The "window resized" event
   When you get this event, you are responsible for setting a new video
   mode with the new width and height.
 */
//typedef struct SDL_ResizeEvent {
//	u8 type; /* SDL_VIDEORESIZE */
//	int w;		/* New width */
//	int h;		/* New height */
//} SDL_ResizeEvent;

/* The "screen redraw" event */
typedef struct SDL_ExposeEvent
{
	u8 type; /* SDL_VIDEOEXPOSE */
} SDL_ExposeEvent;

/* The "quit requested" event */
typedef struct SDL_QuitEvent
{
	u8 type; /* SDL_QUIT */
} SDL_QuitEvent;

/* A user-defined event type */
typedef struct SDL_UserEvent
{
	u8 type; /* SDL_USEREVENT through SDL_NUMEVENTS-1 */
	int code;	/* User defined event code */
	void *data1;	/* User defined data pointer */
	void *data2;	/* User defined data pointer */
} SDL_UserEvent;

/* If you want to use this event, you should include SDL_syswm.h */
//struct SDL_SysWMmsg;
//typedef struct SDL_SysWMmsg SDL_SysWMmsg;
//typedef struct SDL_SysWMEvent {
//	u8 type;
//	SDL_SysWMmsg *msg;
//} SDL_SysWMEvent;

/* General event structure */
typedef union
{
	u8 type;
	SDL_ActiveEvent active;
//	SDL_ResizeEvent resize;
	SDL_ExposeEvent expose;
	SDL_QuitEvent quit;
	SDL_UserEvent user;
//	SDL_SysWMEvent syswm;
} SDL_Event;


/* Function prototypes */

/* Checks the event queue for messages and optionally returns them.
   If 'action' is SDL_ADDEVENT, up to 'numevents' events will be added to
   the back of the event queue.
   If 'action' is SDL_PEEKEVENT, up to 'numevents' events at the front
   of the event queue, matching 'mask', will be returned and will not
   be removed from the queue.
   If 'action' is SDL_GETEVENT, up to 'numevents' events at the front
   of the event queue, matching 'mask', will be returned and will be
   removed from the queue.
   This function returns the number of events actually stored, or -1
   if there was an error.  This function is thread-safe.
*/
typedef enum
{
	SDL_ADDEVENT,
	SDL_PEEKEVENT,
	SDL_GETEVENT
} SDL_eventaction;

/*
  This function allows you to set the state of processing certain events.
  If 'state' is set to SDL_IGNORE, that event will be automatically dropped
  from the event queue and will not event be filtered.
  If 'state' is set to SDL_ENABLE, that event will be processed normally.
  If 'state' is set to SDL_QUERY, SDL_EventState() will return the
  current processing state of the specified event.
*/
#define SDL_QUERY	-1
#define SDL_IGNORE	 0
#define SDL_DISABLE  0
#define SDL_ENABLE	 1
extern u8 /*SD LCALL*/ SDL_EventState(u8 type, int state);


/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
//#inc lude "clo se_code.h"

#endif /* _PSPL_events_h */
