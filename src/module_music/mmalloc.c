
#include "mohou_mikmod_internals.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	-------------------------------------------------------
	MikMod sound library (c) 1998, 1999, 2000 Miodrag Vallat and others.
	-------------------------------------------------------
	$Id: mmalloc.c,v 1.3 1999/12/28 18:51:11 hercules Exp $
	Dynamic memory routines
---------------------------------------------------------*/

/* Same as calloc, but sets error variable mod_music_error when fails */
//void* mod_music_calloc(size_t nitems, size_t size)
void* module_music_calloc(size_t my_size)
{
	void *d;
//	d = calloc(nitems, size);
//	d = calloc(my_size, sizeof(u8));
	d = calloc(my_size, (1));
//	if (NULL == d)
//	{
//		error(FATAL, "music module error. メモリが足りません。");
//		mod_music_error_number = MOD_MUSIC_ERROR_OUT_OF_MEMORY;
//		if (mod_music_error_handler) mod_music_error_handler();
//	}
	return (d);
}

/* Same as malloc, but sets error variable mod_music_error when fails */
//void* mod_music_malloc(size_t size)
//{
//	size_t nitems = 1;
//	void *d;
//	if (!(d=calloc(nitems, size)))
//	{
//		mod_music_error_number = MOD_MUSIC_ERROR_OUT_OF_MEMORY;
//		if (mod_music_error_handler) mod_music_error_handler();
//	}
//	return (d);
//}

/* ex:set ts=4: */
