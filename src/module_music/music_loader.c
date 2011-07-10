
#include "mohou_mikmod_internals.h"

/*---------------------------------------------------------
	東方模倣風 ～ Toho Imitation Style.
	プロジェクトページ http://code.google.com/p/kene-touhou-mohofu/
	-------------------------------------------------------
	-------------------------------------------------------
	MikMod sound library (c) 1998, 1999, 2000 Miodrag Vallat and others.
 	-------------------------------------------------------
	$Id: mlreg.c,v 1.4 2000/11/17 21:38:05 hercules Exp $
	Routine for registering all loaders in libmikmod for the current platform.
---------------------------------------------------------*/

	#ifdef LINK_MOD
		#define ML0000
		#define ML0010
	//	#define ML0011
	//	#define ML0012
	//	#define ML0013
	//	#define ML0020
	//	#define ML0021
	//	#define ML0022
	//	#define ML0023
	//	#define ML0030
	//	#define ML0031
	//	#define ML0032
	//	#define ML0033
	//	#define ML0040
	//	#define ML0041
	//	#define ML0042
	//	#define ML0043
	#endif // LINK_IT

	#ifdef LINK_XM
		#define ML0000
	//	#define ML0010
		#define ML0011
	//	#define ML0012
		#define ML0013
	//	#define ML0020
	//	#define ML0021
	//	#define ML0022
	//	#define ML0023
	//	#define ML0030
	//	#define ML0031
	//	#define ML0032
		#define ML0033
	//	#define ML0040
	//	#define ML0041
	//	#define ML0042
	//	#define ML0043
	#endif // LINK_IT

	#ifdef LINK_S3M
		#define ML0000
	//	#define ML0010
		#define ML0011
	//	#define ML0012
		#define ML0013
	//	#define ML0020
	//	#define ML0021
	//	#define ML0022
	//	#define ML0023
	//	#define ML0030
	//	#define ML0031
	//	#define ML0032
		#define ML0033
	//	#define ML0040
	//	#define ML0041
	//	#define ML0042
	//	#define ML0043
	#endif // LINK_IT

	#ifdef LINK_IT
		#define ML0000
	//	#define ML0010
		#define ML0011
	//	#define ML0012
		#define ML0013
	//	#define ML0020
	//	#define ML0021
	//	#define ML0022
	//	#define ML0023
	//	#define ML0030
	//	#define ML0031
	//	#define ML0032
		#define ML0033
	//	#define ML0040
	//	#define ML0041
	//	#define ML0042
		#define ML0043
	#endif // LINK_IT

#if defined(ML0000)
//tern int mod_music_file_read_string(MF_READER *reader, MM_CHAR *buffer, int number);
static int mod_music_file_read_string(MF_READER *reader, MM_CHAR* buffer, int number)
{
	return (reader->Read(reader, buffer, number));
}
#endif


#if defined(ML0010)
static u16 mod_music_file_read_u16BE(MF_READER* reader)
{
	u16 result = (((u16)(u8)(reader->Get(reader)))<<8);
	result |= (u8)(reader->Get(reader));
	return (result);
}
#endif

#if defined(ML0011)
static u16 mod_music_file_read_u16LE(MF_READER* reader)
{
	u16 result = (u8)(reader->Get(reader));
	result |= (((u16)(u8)(reader->Get(reader)))<<8);
	return (result);
}
#endif

#if defined(ML0012)
static u32 mod_music_file_read_u32BE(MF_READER* reader)
{
	u32 result = (((u32)mod_music_file_read_u16BE(reader))<<16);
	result |= mod_music_file_read_u16BE(reader);
	return (result);
}
#endif

#if defined(ML0013)
static u32 mod_music_file_read_u32LE(MF_READER* reader)
{
	u32 result = mod_music_file_read_u16LE(reader);
	result |= (((u32)mod_music_file_read_u16LE(reader))<<16);
	return (result);
}
#endif

#if defined(ML0020)
static s16 mod_music_file_read_s16BE(MF_READER* reader)
{
	return ((s16)mod_music_file_read_u16BE(reader));
}
#endif

#if defined(ML0021)
static s16 mod_music_file_read_s16LE(MF_READER* reader)
{
	return ((s16)mod_music_file_read_u16LE(reader));
}
#endif

#if defined(ML0022)
static s32 mod_music_file_read_s32BE(MF_READER* reader)
{
	return ((s32)mod_music_file_read_u32BE(reader));
}
#endif

#if defined(ML0023)
static s32 mod_music_file_read_s32LE(MF_READER* reader)
{
	return ((s32)mod_music_file_read_u32LE(reader));
}
#endif


///////////////////////

#ifdef __STDC__
#define DEFINE_MULTIPLE_READ_FUNCTION(type_name, type)										\
static int mod_music_file_read_multi_##type_name(MF_READER* reader, type *buffer, int number)		\
{																							\
	while (number-->0)																		\
	{	*(buffer++) = mod_music_file_read_##type_name(reader);		}							\
	return (!reader->Eof(reader));	\
}
#else
#define DEFINE_MULTIPLE_READ_FUNCTION(type_name, type)										\
static int mod_music_file_read_multi_/**/type_name/**/(MF_READER* reader, type *buffer, int number) 	\
{																							\
	while (number-->0)																		\
	{	*(buffer++) = mod_music_file_read_/**/type_name(reader); 	}						\
	return (!reader->Eof(reader));	\
}
#endif

#if defined(ML0030)
DEFINE_MULTIPLE_READ_FUNCTION(s16BE,s16)
#endif
#if defined(ML0031)
DEFINE_MULTIPLE_READ_FUNCTION(u16BE,u16)
#endif
#if defined(ML0032)
DEFINE_MULTIPLE_READ_FUNCTION(s16LE,s16)
#endif
#if defined(ML0033)
DEFINE_MULTIPLE_READ_FUNCTION(u16LE,u16)
#endif

#if defined(ML0040)
DEFINE_MULTIPLE_READ_FUNCTION(s32BE,s32)
#endif
#if defined(ML0041)
DEFINE_MULTIPLE_READ_FUNCTION(u32BE,u32)
#endif
#if defined(ML0042)
DEFINE_MULTIPLE_READ_FUNCTION(s32LE,s32)
#endif
#if defined(ML0043)
DEFINE_MULTIPLE_READ_FUNCTION(u32LE,u32)
#endif




	#ifdef LINK_MOD
		#include "load_mod_inc.h"
	#endif // LINK_MOD
	#ifdef LINK_XM
		#include "load_xm_inc.h"
	#endif // LINK_XM
//
	#ifdef LINK_S3M
		#include "load_s3m_inc.h"
	#endif // LINK_S3M
	#ifdef LINK_IT
		#include "load_it_inc.h"
	#endif // LINK_IT
void MusicMod_RegisterAllLoaders_internal(void)
{
#if 1/* support majors format only. */
	#ifdef LINK_MOD
	mod_music_registerloader(&load_mod);
	#endif // LINK_MOD
	#ifdef LINK_XM
	mod_music_registerloader(&load_xm);
	#endif // LINK_XM
//
	#ifdef LINK_S3M
	mod_music_registerloader(&load_s3m);
	#endif // LINK_S3M
	#ifdef LINK_IT
	mod_music_registerloader(&load_it);
	#endif // LINK_IT
//
#else /* not support */
	/* do *NOT* linkage minor format */
	mod_music_registerloader(&load_669);	/* not support */
	mod_music_registerloader(&load_amf);	/* not support */
	mod_music_registerloader(&load_dsm);	/* not support */
	mod_music_registerloader(&load_far);	/* not support */
	mod_music_registerloader(&load_gdm);	/* not support */
	mod_music_registerloader(&load_it); 	/* support */
	mod_music_registerloader(&load_imf);	/* not support */
	mod_music_registerloader(&load_mod);	/* support */
	mod_music_registerloader(&load_med);	/* not support */
	mod_music_registerloader(&load_mtm);	/* not support */
	mod_music_registerloader(&load_s3m);	/* support */
	mod_music_registerloader(&load_stm);	/* not support */
	mod_music_registerloader(&load_stx);	/* not support */
	mod_music_registerloader(&load_ult);	/* not support */
	mod_music_registerloader(&load_uni);	/* not support */
	mod_music_registerloader(&load_xm); 	/* support */

	mod_music_registerloader(&load_m15);	/* not support */
#endif
}

void MusicMod_RegisterAllLoaders(void)
{
	MUTEX_LOCK(lists);
	MusicMod_RegisterAllLoaders_internal();
	MUTEX_UNLOCK(lists);
}

void MusicMod_UnregisterAllLoaders(void)
{
	MUTEX_LOCK(lists);
	mod_music_unregisterloaders();
	MUTEX_UNLOCK(lists);
}

#if 0
extern s16 mod_music_file_read_s16BE(MF_READER *reader);
extern s16 mod_music_file_read_s16LE(MF_READER *reader);
extern u16 mod_music_file_read_u16BE(MF_READER *reader);
extern u16 mod_music_file_read_u16LE(MF_READER *reader);

extern s32 mod_music_file_read_s32BE(MF_READER *reader);
extern s32 mod_music_file_read_s32LE(MF_READER *reader);
extern u32 mod_music_file_read_u32BE(MF_READER *reader);
extern u32 mod_music_file_read_u32LE(MF_READER *reader);

extern int mod_music_file_read_multi_s16BE(MF_READER *reader, s16 *buffer, int number);
extern int mod_music_file_read_multi_s16LE(MF_READER *reader, s16 *buffer, int number);
extern int mod_music_file_read_multi_u16BE(MF_READER *reader, u16 *buffer, int number);
extern int mod_music_file_read_multi_u16LE(MF_READER *reader, u16 *buffer, int number);

extern int mod_music_file_read_multi_s32BE(MF_READER *reader, s32 *buffer, int number);
extern int mod_music_file_read_multi_s32LE(MF_READER *reader, s32 *buffer, int number);
extern int mod_music_file_read_multi_u32BE(MF_READER *reader, u32 *buffer, int number);
extern int mod_music_file_read_multi_u32LE(MF_READER *reader, u32 *buffer, int number);
#endif

/* ex:set ts=4: */
