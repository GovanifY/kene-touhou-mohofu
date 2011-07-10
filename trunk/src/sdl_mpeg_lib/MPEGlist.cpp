
#include "MPEGlist.h"
#include <malloc.h>
extern "C" {void error(int errorlevel, char *msg, ...);}

MPEGlist::MPEGlist()
{
	size = 0;
	data = 0;
	lock = 0;
	next = 0;
	prev = 0;
	TimeStamp = -1;
}

MPEGlist::~MPEGlist()
{
	if (next)	{	next->prev = prev;}
	if (prev)	{	prev->next = next;}
	if (data)
	{
		free(data); 	//	delete[] data;
		data = 0;
	}
}

/* Return the next free buffer or allocate a new one if none is empty */
MPEGlist * MPEGlist::Alloc(u32 Buffer_Size)
{
	MPEGlist * tmp;
	tmp 		= next;
	next		= new MPEGlist;
	next->next	= tmp;
	if ( Buffer_Size )
	{
		next->data = (u8 *)malloc(Buffer_Size);	//	next->data = new u8[Buffer_Size];
		if (!next->data)
		{
			//#ifdef PSP_VIRTUAL_CONSOLE_IO
			error(0/*ERR_FATAL*/, "smpeg: Not enough memory");
			//error(0/*ERR_FATAL*/, "Alloc : Not enough memory\n");
			//#endif
			return (0);
		}
	}
	else
	{
		next->data = 0;
	}
	next->size = Buffer_Size;
	next->prev = this;
	return (next);
}

/* Lock current buffer */
void MPEGlist::Lock()
{
	lock++;
}

/* Unlock current buffer */
void MPEGlist::Unlock()
{
	if (lock != 0)	{lock--;}
}
